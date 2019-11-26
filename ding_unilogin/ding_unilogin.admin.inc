<?php

/**
 * @file
 * Implements the administration UI for the module.
 */

/**
 * UNI•Login settings form.
 */
function ding_unilogin_admin_settings_form($form, &$form_state) {
  // Settings for the basic login service.
  $form['login'] = array(
    '#type' => 'fieldset',
    '#title' => t('UNI•Login'),
    '#description' => t('Settings for the base login service.'),
  );

  $form['login']['ding_unilogin_publizon_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Publizon Client ID'),
    '#description' => t('The client ID to send to publizon, for UNI•Login users.'),
    '#default_value' => variable_get('ding_unilogin_publizon_client_id', ''),
    '#required' => TRUE,
  );

  $form['login']['ding_unilogin_id'] = array(
    '#type' => 'textfield',
    '#title' => t('UNI•Login ID'),
    '#description' => t('The UNI•Login ID. Leave empty to disable UNI•Login.'),
    '#default_value' => variable_get('ding_unilogin_id', ''),
  );

  $form['login']['ding_unilogin_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('UNI•Login shared secret'),
    '#description' => t('The shared secret used to calculate hashes.'),
    '#default_value' => variable_get('ding_unilogin_secret', ''),
  );

  $form['login']['ding_unilogin_global_logout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Global logout'),
    '#description' => t('Log out of UNI•Login when logging out of Drupal.'),
    '#default_value' => variable_get('ding_unilogin_global_logout', 0),
  );

  // Settings for WS service general access.
  $form['ws'] = array(
    '#type' => 'fieldset',
    '#title' => t('UNI•Login WS common'),
    '#description' => t('Common settings for the additional information web services.'),
  );

  $form['ws']['ding_unilogin_ws_user'] = array(
    '#type' => 'textfield',
    '#title' => t('UNI•Login webservice user ID'),
    '#default_value' => variable_get('ding_unilogin_ws_user', ''),
  );

  $form['ws']['ding_unilogin_ws_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('UNI•Login webservice password'),
    '#default_value' => variable_get('ding_unilogin_ws_pass', ''),
  );

  // Settings for auth settings.
  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('UNI•Login Auth'),
    '#description' => t('Settings for the authentication web services.'),
  );

  $form['auth']['ding_unilogin_auth_context'] = array(
    '#type' => 'textfield',
    '#title' => t('Context'),
    '#default_value' => variable_get('ding_unilogin_auth_context', ''),
    '#description' => t('The WS05 context to check for access in. If no context is given, all valid UNI•Login users are granted access.'),
  );

  $form['auth']['ding_unilogin_auth_project'] = array(
    '#type' => 'textfield',
    '#title' => t('Project'),
    '#default_value' => variable_get('ding_unilogin_auth_project', ''),
    '#description' => t('The WS05 project to check for access to.'),
  );

  // Make the form look like at system form.
  $form = system_settings_form($form);

  return $form;
}

/**
 * UNI•Login institutions form.
 */
function ding_unilogin_admin_institutions_form($form, &$form_state) {
  $institutions = variable_get('ding_unilogin_institutions', []);

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
    '#description' => t("If set, a user's list of institutions will be filtered by the list below when signing in via UNI•Login."),
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
 * Theme for UNI•Login institutions form.
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
 * Submit handler for UNI•Login institutions form.
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
  variable_set('ding_unilogin_institutions', $institutions);
}
