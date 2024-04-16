<?php

/**
 * @file
 * Implements the administration UI for the module.
 */

/**
 * Unilogin settings form.
 */
function ding_unilogin_admin_settings_form($form, &$form_state) {
  // Settings for the basic login service.
  $form['login'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unilogin'),
    '#description' => t('Settings for the base login service.'),
  );

  $form['login']['ding_unilogin_publizon_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Publizon Client ID'),
    '#description' => t('The client ID to send to publizon, for Unilogin users.'),
    '#default_value' => variable_get('ding_unilogin_publizon_client_id', ''),
    '#required' => TRUE,
  );

  $form['login']['ding_unilogin_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Unilogin ID'),
    '#description' => t('The Unilogin ID. Leave empty to disable Unilogin.'),
    '#default_value' => variable_get('ding_unilogin_id', ''),
  );

  $form['login']['ding_unilogin_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Unilogin shared secret'),
    '#description' => t('The shared secret used to calculate hashes.'),
    '#default_value' => variable_get('ding_unilogin_secret', ''),
  );

  $form['login']['ding_unilogin_global_logout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Global logout'),
    '#description' => t('Log out of Unilogin when logging out of Drupal.'),
    '#default_value' => variable_get('ding_unilogin_global_logout', 0),
  );

  // Settings for WS service general access.
  $form['ws'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unilogin WS common'),
    '#description' => t('Common settings for the additional information web services.'),
  );

  $form['ws']['ding_unilogin_ws_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Unilogin webservice user ID'),
    '#default_value' => variable_get('ding_unilogin_ws_user', ''),
  );

  $form['ws']['ding_unilogin_ws_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Unilogin webservice password'),
    '#default_value' => variable_get('ding_unilogin_ws_pass', ''),
  );

  // Settings for auth settings.
  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unilogin Auth'),
    '#description' => t('Settings for the authentication web services.'),
  );

  $configuration = _ding_unilogin_get_unilogin_helper()->getConfiguration();
  $form['auth']['ding_unilogin_oidc'] = [
    '#type' => 'fieldset',
    '#title' => t('OIDC'),
    '#tree' => TRUE,
    '#description' => t('Settings for <a href="!url">Nyt Unilogin (OIDC)</a>', [
      '!url' => 'https://viden.stil.dk/display/OFFSKOLELOGIN/OIDC',
    ]),
  ];

  $form['auth']['ding_unilogin_oidc']['client_id'] = [
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#size' => 36,
    '#default_value' => $configuration['client_id'] ?? NULL,
    '#required' => TRUE,
  ];

  $form['auth']['ding_unilogin_oidc']['client_secret'] = [
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#size' => 36,
    '#default_value' => $configuration['client_secret'] ?? NULL,
    '#required' => TRUE,
  ];

  $form['auth']['ding_unilogin_oidc']['oidc_discovery_url'] = [
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

  $form['auth']['ding_unilogin_oidc']['oidc'] = [
    '#type' => 'hidden',
    '#title' => 'OIDC data',
    '#default_value' => json_encode($configuration['oidc'] ?? (object) []),
  ];

  if (isset($configuration['oidc'], $configuration['oidc_discovery_url'])) {
    $form['auth']['ding_unilogin_oidc']['oidc_data'] = [
      '#type' => 'fieldset',
      '#title' => t('OIDC data'),
      '#description' => t('Loaded from <a href="!url">!url</a>', ['!url' => $configuration['oidc_discovery_url']]),

      'data' => [
        '#markup' => json_encode($configuration['oidc'], JSON_PRETTY_PRINT | JSON_UNESCAPED_SLASHES),
        '#prefix' => '<pre style="max-height: 20em; overflow: scroll">',
        '#suffix' => '</pre>',
      ],
    ];
  }

  $form['#validate'][] = 'ding_unilogin_admin_settings_form_validate_oidc';

  // Miscellaneus settings.
  $form['misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneus'),
    '#description' => t('Miscellaneus settings.'),
  );

  $form['misc']['ding_unilogin_error_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Error paths'),
    '#default_value' => variable_get('ding_unilogin_error_paths', ''),
    '#description' => t('Error paths (e.g. node/65) that should not be accessible by authenticated users unless they have edit access. One path per line.'),
  );

  $form['misc']['ding_unilogin_error_paths_redirect_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Error paths redirect path'),
    '#default_value' => variable_get('ding_unilogin_error_paths_redirect_path', 'user'),
    '#description' => t('The path to redirect authenticated users to when they access an error path.'),
  );

  // Make the form look like at system form.
  $form = system_settings_form($form);

  return $form;
}

/**
 * Validation function for (mis)used to discover OIDC data.
 */
function ding_unilogin_admin_settings_form_validate_oidc(&$form, &$form_state) {
  $oidc_discovery_url = trim($form_state['values']['ding_unilogin_oidc']['oidc_discovery_url'] ?? '');
  if (valid_url($oidc_discovery_url)) {
    $response = drupal_http_request($oidc_discovery_url);
    if (200 === (int) ($response->code ?? NULL)) {
      try {
        $data = json_decode($response->data, TRUE, 512, JSON_THROW_ON_ERROR);
        form_set_value($form['auth']['ding_unilogin_oidc']['oidc'], $data,
          $form_state);

        $required_keys = [
          'authorization_endpoint',
          'token_endpoint',
          'token_introspection_endpoint',
        ];
        foreach ($required_keys as $key) {
          if (!isset($data[$key])) {
            form_set_error('auth][ding_unilogin_oidc][oidc_discovery_url',
              t('Key "@key" missing in OIDC discovery data', ['@key' => $key]));
          }
        }
      }
      catch (JsonException $jsonException) {
        form_set_error('auth][ding_unilogin_oidc][oidc_discovery_url', t('Error parsing OIDC discovery document: %message', ['%message' => $jsonException->getMessage()]));
      }
    }
    else {
      form_set_error('auth][ding_unilogin_oidc][oidc_discovery_url', t('Cannot load content from OIDC discovery URL'));
    }
  }
  else {
    form_set_error('auth][ding_unilogin_oidc][oidc_discovery_url', t('Please enter a valid OIDC discovery URL'));
  }
}

/**
 * Unilogin api form.
 */
function ding_unilogin_admin_api_form($form, &$form_state) {
  $form['api']['ding_unilogin_api_token_read'] = array(
    '#type' => 'textfield',
    '#title' => t('Api token (read)'),
    // Elvis operator used to handle falsy values.
    '#default_value' => variable_get('ding_unilogin_api_token_read'),
    '#description' => t('Token needed to access the api. Leave empty to generate a random token.'),
  );

  $form['api']['ding_unilogin_api_token_write'] = array(
    '#type' => 'textfield',
    '#title' => t('Api token (write)'),
    // Elvis operator used to handle falsy values.
    '#default_value' => variable_get('ding_unilogin_api_token_write'),
    '#description' => t('Token needed to access the api. Leave empty to generate a random token.'),
  );

  $form['api']['ding_unilogin_api_allowed_http_origins'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed http origins (one per line)'),
    '#default_value' => variable_get('ding_unilogin_api_allowed_http_origins', 'https://givadgang.uni-login.dk'),
    '#description' => t('Allowed http origins, e.g. <pre>https://givadgang.uni-login.dk</pre>'),
  );

  $form['ding_unilogin_api_endpoints'] = array(
    '#type' => 'fieldset',
    '#title' => t('Api endpoints'),
    'content' => array(
      '#markup' => implode('', array(
        '<ul>',
        '<li>' . l(t('Institutions'), '/unilogin/api/institutions') . '</li>',
        '<li>' . l(t('Municipalities'), '/unilogin/api/municipalities') . '</li>',
        '</ul>',
      )),
    ),
  );

  _ding_unilogin_admin_bookmarklet($form);

  // Important: Our custom submit hook must be added before the ones added by
  // `system_settings_form`.
  $form['#submit'][] = 'ding_unilogin_admin_api_form_submit';
  $form = system_settings_form($form);

  return $form;
}

/**
 * Unilogin institutions form.
 */
function ding_unilogin_admin_institutions_form($form, &$form_state) {
  $institutions = _ding_unilogin_get_institutions();

  // Check that the huge form below can actually be handled.
  $max_input_vars_current_value = (int) ini_get('max_input_vars');
  $max_input_vars_recommended_value = (int) 4 * count($institutions) + 100;

  if ($max_input_vars_current_value < $max_input_vars_recommended_value) {
    drupal_set_message(t('The value of the PHP runtime variable <a href="!doc_url"><code>max_input_vars</code></a> is too low. The current value is <code>!current_value</code>, but it should be at least <code>!recommended_value</code>.', array(
      '!doc_url' => 'https://www.php.net/manual/en/info.configuration.php#ini.max-input-vars',
      '!current_value' => $max_input_vars_current_value,
      '!recommended_value' => $max_input_vars_recommended_value,
    )), 'error');
  }

  $form['ding_unilogin_institutions_apply_filter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply institutions filter'),
    '#default_value' => variable_get('ding_unilogin_institutions_apply_filter', FALSE),
    '#description' => t("If set, a user's list of institutions will be filtered by the list below when signing in via Unilogin."),
  );

  $form['ding_unilogin_institutions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Institutions'),
    '#tree' => TRUE,
    '#description' => t('Clear the institution id to remove a row.'),
  );

  // Add empty row.
  $institutions['__new__'] = array(
    'id' => '',
  );

  foreach ($institutions as $index => $institution) {
    $form['ding_unilogin_institutions'][$index] = array(
      'id' => array(
        '#type' => 'textfield',
        '#size' => 10,
        '#title' => t('Id'),
        '#default_value' => $institution['id'] ?? NULL,
      ),
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => $institution['name'] ?? NULL,
      ),
      'number_of_members' => array(
        '#type' => 'textfield',
        '#size' => 10,
        '#title' => t('Number of members'),
        '#default_value' => $institution['number_of_members'] ?? NULL,
      ),
      'group' => array(
        '#type' => 'textfield',
        '#title' => t('Group'),
        '#default_value' => $institution['group'] ?? NULL,
      ),
      'type' => array(
        '#type' => 'textfield',
        '#title' => t('Type'),
        '#default_value' => $institution['type'] ?? NULL,
      ),
    );
  }

  $form['#theme'] = 'ding_unilogin_admin_institutions_form';
  $form = system_settings_form($form);
  $form['#submit'] = array('ding_unilogin_admin_institutions_form_submit');

  return $form;
}

/**
 * Theme for Unilogin institutions form.
 *
 * Renders fields of each institution as rows in a table.
 */
function theme_ding_unilogin_admin_institutions_form($variables) {
  $form = $variables['form'];

  // Render institutions as table.
  $header = array();
  $rows = array();
  $first_row = TRUE;
  foreach (element_children($form['ding_unilogin_institutions']) as $key) {
    $row = array();
    foreach (element_children($form['ding_unilogin_institutions'][$key]) as $field) {
      if ($first_row) {
        $header[] = $form['ding_unilogin_institutions'][$key][$field]['#title'];
      }
      $form['ding_unilogin_institutions'][$key][$field]['#title_display'] = 'invisible';
      $row[] = drupal_render($form['ding_unilogin_institutions'][$key][$field]);
    }
    $rows[] = $row;
    $first_row = FALSE;
  }
  $form['ding_unilogin_institutions']['#children'] = theme('table', array('header' => $header, 'rows' => $rows));

  // Render the form.
  return drupal_render_children($form);
}

/**
 * Submit handler for Unilogin institutions form.
 */
function ding_unilogin_admin_institutions_form_submit(array $form, array &$form_state) {
  $values = $form_state['values'];

  // Get institutions with id.
  $institutions = array_filter($values['ding_unilogin_institutions'], static function (array $institution) {
    return $institution['id'];
  });
  // Note: PHP will work very hard to convert numeric keys to integer keys.
  $institutions = array_column($institutions, NULL, 'id');

  variable_set('ding_unilogin_institutions_apply_filter', (bool) $values['ding_unilogin_institutions_apply_filter']);
  _ding_unilogin_set_institutions($institutions);
}

/**
 * Unilogin nonstitutions form.
 */
function ding_unilogin_admin_nonstitutions_form($form, &$form_state) {
  $nonstitutions = _ding_unilogin_get_nonstitutions();

  // Check that the huge form below can actually be handled.
  $max_input_vars_current_value = (int) ini_get('max_input_vars');
  $max_input_vars_recommended_value = (int) 4 * count($nonstitutions) + 100;

  if ($max_input_vars_current_value < $max_input_vars_recommended_value) {
    drupal_set_message(t('The value of the PHP runtime variable <a href="!doc_url"><code>max_input_vars</code></a> is too low. The current value is <code>!current_value</code>, but it should be at least <code>!recommended_value</code>.', array(
      '!doc_url' => 'https://www.php.net/manual/en/info.configuration.php#ini.max-input-vars',
      '!current_value' => $max_input_vars_current_value,
      '!recommended_value' => $max_input_vars_recommended_value,
    )), 'error');
  }

  $form['ding_unilogin_nonstitutions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nonstitutions'),
    '#tree' => TRUE,
    '#description' => t('Clear the institution id to remove a row.'),
  );

  // Add empty row.
  $nonstitutions['__new__'] = array(
    'id' => '',
  );

  foreach ($nonstitutions as $index => $nonstitution) {
    $form['ding_unilogin_nonstitutions'][$index] = array(
      'institution_id' => array(
        '#type' => 'textfield',
        '#size' => 10,
        '#title' => t('Institution id'),
        '#default_value' => $nonstitution['institution_id'] ?? NULL,
      ),
      'municipality_kommunenr' => array(
        '#type' => 'textfield',
        '#size' => 10,
        '#title' => t('Municipality id'),
        '#default_value' => $nonstitution['municipality_kommunenr'] ?? NULL,
      ),
      'municipality_kommune' => array(
        '#type' => 'textfield',
        '#title' => t('Municipality name'),
        '#default_value' => $nonstitution['municipality_kommune'] ?? NULL,
      ),
      'test' => array(
        '#type' => 'container',
        '#title' => t('Test (using saved data)'),
      ),
    );

    if (isset($nonstitution['institution_id'])) {
      $test_url = 'unilogin/api/institutions/' . $nonstitution['institution_id'];
      $form['ding_unilogin_nonstitutions'][$index]['test']['link'] = array(
        '#type' => 'link',
        '#title' => $test_url,
        '#href' => $test_url,
        '#attributes' => [
          'target' => 'ding_unilogin_nonstitutions',
        ],
      );
    }
  }

  $form['institutions_with_no_municipality_id'] = [
    '#type' => 'fieldset',
    '#title' => t('Institutions with no municipality id'),
    '#weight' => 1000,
  ];

  $institutions_with_no_municipality_id = array_filter(
    _ding_unilogin_get_institutions(TRUE),
    static function (array $institution) {
      return !isset($institution['municipality']->kommunenr);
    }
  );

  if (empty($institutions_with_no_municipality_id)) {
    $form['institutions_with_no_municipality_id']['list']['#markup'] = t('No institutions are missing municipality id');
  }
  else {
    $header = array(
      t('Institution id'),
      t('Name'),
      t('Municipality'),
    );
    $rows = array_map(static function (array $institution) {
      return array(
        $institution['id'],
        $institution['name'],
        sprintf('%s %s', $institution['municipality']->postnr ?? NULL, $institution['municipality']->bynavn ?? NULL),
      );
    }, $institutions_with_no_municipality_id);

    $form['institutions_with_no_municipality_id']['list']['#markup'] = theme('table', array('header' => $header, 'rows' => $rows));
  }

  $form['test'] = [
    '#type' => 'fieldset',
    '#title' => t('Test'),
    '#weight' => 1001,

    'all_institutions' => [
      '#type' => 'link',
      '#title' => t('List all institutions (API)'),
      '#href' => 'unilogin/api/institutions/',
      '#attributes' => [
        'target' => 'ding_unilogin_nonstitutions',
      ],
    ],
  ];

  $form['#theme'] = 'ding_unilogin_admin_nonstitutions_form';
  $form['#validate'] = array('ding_unilogin_admin_nonstitutions_form_validate');
  $form = system_settings_form($form);
  $form['#submit'] = array('ding_unilogin_admin_nonstitutions_form_submit');

  return $form;
}

/**
 * Theme for Unilogin nonstitutions form.
 *
 * Renders fields of each nonstitution as rows in a table.
 */
function theme_ding_unilogin_admin_nonstitutions_form($variables) {
  $form = $variables['form'];

  // Render nonstitutions as table.
  $header = array();
  $rows = array();
  $first_row = TRUE;
  foreach (element_children($form['ding_unilogin_nonstitutions']) as $key) {
    $row = array();
    foreach (element_children($form['ding_unilogin_nonstitutions'][$key]) as $field) {
      if ($first_row) {
        $header[] = $form['ding_unilogin_nonstitutions'][$key][$field]['#title'];
      }
      $form['ding_unilogin_nonstitutions'][$key][$field]['#title_display'] = 'invisible';
      $row[] = drupal_render($form['ding_unilogin_nonstitutions'][$key][$field]);
    }
    $rows[] = $row;
    $first_row = FALSE;
  }
  $form['ding_unilogin_nonstitutions']['#children'] = theme('table', array('header' => $header, 'rows' => $rows));

  // Render the form.
  return drupal_render_children($form);
}

/**
 * Validating handler for Unilogin nonstitutions form.
 */
function ding_unilogin_admin_nonstitutions_form_validate(array $form, array &$form_state) {
  $institutions = _ding_unilogin_get_institutions();
  $values = $form_state['values'];
  foreach (element_children($form['ding_unilogin_nonstitutions']) as $child) {
    $row = $values['ding_unilogin_nonstitutions'][$child];
    if (!empty($row['institution_id'])) {
      if (!isset($institutions[$row['institution_id']])) {
        form_set_error(
          'ding_unilogin_nonstitutions][' . $child . '][institution_id',
          t('Invalid institution id: @institution_id', ['@institution_id' => $row['institution_id']])
        );
      }
      if (empty($row['municipality_kommunenr'])) {
        form_set_error(
          'ding_unilogin_nonstitutions][' . $child . '][municipality_kommunenr',
          t('Municipality id cannot be empty')
        );
      }
    }
  }
}

/**
 * Submit handler for Unilogin nonstitutions form.
 */
function ding_unilogin_admin_nonstitutions_form_submit(array $form, array &$form_state) {
  $values = $form_state['values'];

  // Get nonstitutions with non-empty institution id.
  $nonstitutions = array_filter($values['ding_unilogin_nonstitutions'], static function (array $nonstitution) {
    return $nonstitution['institution_id'];
  });
  // Note: PHP will work very hard to convert numeric keys to integer keys.
  $nonstitutions = array_column($nonstitutions, NULL, 'institution_id');

  _ding_unilogin_set_nonstitutions($nonstitutions);
}

/**
 * Submit handler for Unilogin api form.
 */
function ding_unilogin_admin_api_form_submit(array $form, array &$form_state) {
  $values = &$form_state['values'];

  $fields = ['ding_unilogin_api_token_read', 'ding_unilogin_api_token_write'];
  foreach ($fields as $field) {
    if (empty(trim($values[$field]))) {
      $values[$field] = sha1(random_bytes(32));
    }
  }
}

/**
 * Bookmarklet for exporting institutions.
 *
 * Until recently, people at eReolen used a Chrome extension
 * (https://github.com/rimi-itk/ereolengo-givadgang/) to scrape and export
 * institutions from givadgang.dk to eReolen GO, but Københavns kommune does not
 * allow users to install browser extensions.
 *
 * Therefore, we use a bookmarklet as a (temporary?) workaround.
 *
 * @param array $form
 *   The form.
 */
function _ding_unilogin_admin_bookmarklet(array &$form) {
  $api_url = url('/unilogin/api/institutions', ['absolute' => TRUE]);
  $api_token = variable_get('ding_unilogin_api_token_write');

  if ($api_token) {
    // JavaScript to inject a script on the page.
    $js = sprintf(
      <<<'JS'
try {
  // Global variable used by script.
  ereolen_givadgang_api_url = '%s';
  ereolen_givadgang_api_token = '%s';
  s = document.createElement('script');
  // @see https://statically.io/
  s.src = 'https://cdn.statically.io/gh/rimi-itk/ereolengo-givadgang/feature/bookmarklet/script.js';
  document.body.appendChild(s);
} catch (error) {
  alert(error);
}
JS,
      $api_url,
      $api_token,
    );

    $form['ding_unilogin_admin_bookmarklet'] = [
      '#type' => 'fieldset',
      '#title' => t('Giv adgang (bookmarklet)'),

      'intro' => [
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#markup' => t('Drag the following <a href="!bookmarklet_url">bookmarklet</a> to your bookmark bar and save it:', [
          '!bookmarklet_url' => 'https://en.wikipedia.org/wiki/Bookmarklet',
        ]),
      ],

      'bookmarklet' => [
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#markup' => '<a class="button" href="javascript:(function(){'
        . rawurlencode(
          // Remove inline comments and white space.
          preg_replace(['#^\s*//.+#m', '#\s+#'], '', $js)
        )
        . '})();">'
        . t('Export institutions to !host', ['!host' => parse_url($api_url, PHP_URL_HOST)])
        . '</a>',
      ],

      'outtro' => [
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#markup' => t('Activate the bookmarklet when you want to export institutions from <a href="!giv_adgang_url">!giv_adgang_url</a>.', [
          '!giv_adgang_url' => 'https://givadgang.unilogin.dk/#/',
        ]),
      ],

    ];
  }
}
