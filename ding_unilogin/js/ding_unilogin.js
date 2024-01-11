/**
 * @file
 * JS to activate the UNI•Login button.
 */

(function ($, Drupal) {
  'use strict';

  /**
   * Fetch the correct link, update the login link and show it.
   *
   * The login form loaded over AJAX doesn't now the path the user is
   * really on, so we hack it in with JS.
   */
  Drupal.behaviors.ding_unilogin = {
    attach: function (context) {
      $('.unilogin-button', context).once('ding-unilogin', function () {
        var link = $(this);
        var loginUrl = new URL(link.attr('href'), window.location.href);
        var currentUrl = new URL(window.location.href);
        // Add the id of the link clicked to open the popup, so we'll
        // be able to re-trigger it when returning from UNI•Login..
        if (Drupal.ding_unilogin.last_clicked) {
          // We'll encode the id once more to ensure that it'll stay
          // un-decoded all the way back to our triggering JS.
          currentUrl.searchParams.set('ding-unilogin-trigger', Drupal.ding_unilogin.last_clicked);
        }
        loginUrl.searchParams.set('path', currentUrl.toString());
        link.attr('href', loginUrl.toString());
      });
    }
  };
})(jQuery, Drupal);

