<?php

/**
 * @file
 * Defines the administration interface for the module.
 */

/**
 * Defines the administration settings form.
 */
function breol_unilogin_admin_settings_form($form, &$form_state) {
  $form = [
    '#tree' => TRUE,
  ];

  $form['breol_unilogin_settings'] = [
    '#type' => 'fieldset',
    '#title' => t('Unilogin configuration'),
  ];

  $configuration = breol_unilogin_get_configuration();

  $form['breol_unilogin_settings']['client_id'] = [
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#size' => 36,
    '#default_value' => $configuration['client_id'] ?? NULL,
    '#required' => TRUE,
  ];

  $form['breol_unilogin_settings']['client_secret'] = [
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#size' => 36,
    '#default_value' => $configuration['client_secret'] ?? NULL,
    '#required' => TRUE,
  ];

  $form['breol_unilogin_settings']['oidc_discovery_url'] = [
    '#type' => 'textfield',
    '#title' => t('OIDC discovery document URL'),
    '#size' => 128,
    '#default_value' => $configuration['oidc_discovery_url'] ?? NULL,
    '#required' => TRUE,
    '#description' => t('URL to your OIDC discovery document. Hints: !hints', [
      '!hints' => implode(', ', array_map(
        static function (string $url) {
          return sprintf('<a target="oidc_discovery_url" href="%1$s">%1$s</a>', $url);
        },
        [
          'https://et-broker.unilogin.dk/auth/realms/broker/.well-known/openid-configuration',
          'https://broker.unilogin.dk/auth/realms/broker/.well-known/openid-configuration',
        ]
      )),
    ]),
  ];

  $form['breol_unilogin_settings']['oidc'] = [
    '#type' => 'hidden',
    '#title' => 'OIDC data',
    '#default_value' => json_encode($configuration['oidc'] ?? (object) []),
  ];

  if (isset($configuration['oidc'])) {
    $form['breol_unilogin_settings']['oidc_data'] = [
      '#type' => 'fieldset',
      '#title' => t('OIDC data'),

      'data' => [
        '#markup' => json_encode($configuration['oidc'], JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES),
        '#prefix' => '<pre style="max-height: 20em; overflow: scroll">',
        '#suffix' => '</pre>',
      ],
    ];

  }

  $form['#validate'][] = 'breol_unilogin_admin_settings_form_validate';

  return system_settings_form($form);
}

/**
 * Validation function for administration settings form.
 */
function breol_unilogin_admin_settings_form_validate(&$form, &$form_state) {
  $oidc_discovery_url = trim($form_state['values']['breol_unilogin_settings']['oidc_discovery_url'] ?? '');
  if (valid_url($oidc_discovery_url)) {
    $response = drupal_http_request($oidc_discovery_url);
    if (200 == ($response->code ?? NULL)) {
      $data = json_decode($response->data, TRUE);
      form_set_value($form['breol_unilogin_settings']['oidc'], $data, $form_state);

      $required_keys = [
        'authorization_endpoint',
        'token_endpoint',
        'token_introspection_endpoint',
      ];
      foreach ($required_keys as $key) {
        if (!isset($data[$key])) {
          form_set_error('breol_unilogin_settings][oidc_discovery_url',
            t('Key "@key" missing in OIDC discovery data', ['@key' => $key]));
        }
      }
    }
    else {
      form_set_error('breol_unilogin_settings][oidc_discovery_url', t('Cannot load content from OIDC discovery URL'));
    }
  }
  else {
    form_set_error('breol_unilogin_settings][oidc_discovery_url', t('Please enter a valid OIDC discovery URL'));
  }
}
