<?php

/**
 * @file
 * Drush commands for the ding_unilogin module.
 */

/**
 * Implements hook_drush_command().
 */
function ding_unilogin_drush_command() {
  $commands['ding-unilogin-update-municipalities'] = array(
    'description' => 'Updates the list of municipalities with data from STIL.',
    'examples' => array(
      'drush ding-unilogin-update-municipalities' => 'Update municipalities.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  return $commands;
}

/**
 * Update municipalities.
 */
function drush_ding_unilogin_update_municipalities() {
  _ding_unilogin_update_municipalities();
  $municipalities = _ding_unilogin_get_municipalities();
  drush_log(t('Municipalities updated (!count)', ['!count' => count($municipalities)]), 'status');
}
