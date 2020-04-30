; jQuery(function () {
  if (Drupal.settings.breol_app_warning) {
    if (Drupal.settings.breol_app_warning.dialog_options
      && new RegExp(Drupal.settings.breol_app_warning.user_agent_pattern).test(navigator.userAgent)) {
      Drupal.ding_popup.open(Drupal.settings.breol_app_warning.dialog_options);
    }
  }
});

