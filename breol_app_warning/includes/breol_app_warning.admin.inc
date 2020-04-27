<?php

/**
 * @file
 * Admin form for breol_app_warning.
 */

/**
 * The administration form for this module.
 *
 * @param array $form
 *   The form input.
 * @param array $form_state
 *   The state of the form.
 *
 * @return array
 *   The finished form.
 */
function breol_app_warning_admin_form(array $form, array &$form_state) {
  $form['breol_app_warning_popup_title'] = [
    '#type' => 'textfield',
    '#title' => t('Popup title'),
    '#description' => t('The title of the popup.'),
    '#required' => TRUE,
    '#default_value' => variable_get('breol_app_warning_popup_title'),
  ];

  $form['breol_app_warning_popup_content'] = [
    '#type' => 'textarea',
    '#title' => t('Popup content'),
    '#description' => t('The content of the popup.'),
    '#required' => TRUE,
    '#default_value' => variable_get('breol_app_warning_popup_content'),
  ];

  $form['breol_app_warning_popup_user_agent_pattern'] = [
    '#type' => 'textfield',
    '#title' => t('User agent pattern'),
    '#description' => t("The pattern to match against browser's user-agent string. Must be a valid JavaScript regexp pattern."),
    '#required' => TRUE,
    '#default_value' => variable_get('breol_app_warning_popup_user_agent_pattern', 'gonative'),
  ];

  return system_settings_form($form);
}
