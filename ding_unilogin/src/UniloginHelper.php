<?php

namespace Drupal\ding_unilogin;

use Drupal\ding_unilogin\OAuth2\Client\Provider\StilUniloginProvider;

/**
 * Unilogin helper.
 */
class UniloginHelper {

  /**
   * Generate login url endpoint at the IDP.
   *
   * @return string
   *   URL to redirect to at the IDP.
   *
   * @throws Exception
   *   If required libraries are not loaded.
   */
  public function generateLoginUrl() {
    $provider = $this->getProvider();
    $authorization_url = $provider->getAuthorizationUrl([
      // We need an id_token for later use when ending Unilogin session (cf.
      // https://viden.stil.dk/display/OFFSKOLELOGIN/Implementering+af+tjeneste#Implementeringaftjeneste-1.3LogindviaOIDCTjeneste)
      'scope' => 'openid',
    ]);

    // Get the state generated for you and store it to the session.
    $_SESSION['oauth2state'] = $provider->getState();

    // https://viden.stil.dk/display/OFFSKOLELOGIN/Implementering+af+tjeneste#Implementeringaftjeneste-1.3.1Hvordangenerereskorrektcode_challengeogcodeverifier
    // https://medium.com/zenchef-tech-and-product/how-to-generate-a-pkce-challenge-with-php-fbee1fa29379
    $code_verifier = bin2hex(random_bytes(64));
    $hash = hash('sha256', $code_verifier);

    // @see https://base64.guru/developers/php/examples/base64url
    $base64url_encode = function (string $data) {
      // First of all you should encode $data to Base64 string.
      $b64 = base64_encode($data);

      // Make sure you get a valid result, otherwise, return FALSE, as the
      // base64_encode() function does.
      if ($b64 === FALSE) {
        return FALSE;
      }

      // Convert Base64 to Base64URL by replacing “+” with “-” and “/” with “_”.
      $url = strtr($b64, '+/', '-_');

      // Remove padding character from the end of line and return the Base64URL
      // result.
      return rtrim($url, '=');
    };
    $code_challenge = $base64url_encode(pack('H*', $hash));
    $_SESSION['oauth2code_verifier'] = $code_verifier;

    return url($authorization_url, [
      'query' => [
        // https://viden.stil.dk/display/OFFSKOLELOGIN/Implementering+af+tjeneste#Implementeringaftjeneste-1.2EndpointstilOIDCkonfiguration
        'code_challenge' => $code_challenge,
        'code_challenge_method' => 'S256',
      ],
    ]);
  }

  /**
   * Get the authentication configuration.
   *
   * @return array
   *   The configuration.
   */
  public function getConfiguration(): array {
    $config = variable_get('ding_unilogin_oidc', []);

    // Allways set redirect url (editing not allowed in admin).
    $config['redirect_uri'] = url(DING_UNILOGIN_REDIRECT_PATH, ['absolute' => TRUE]);

    // HACK HACK HACK!
    //
    // Remove this when proper redirect URI is enabled.
    // DEBUG
    // For now only the empty path is a valid redirect URI.
    // @see ding_unilogin_boot().
    $config['redirect_uri'] = url('/', ['absolute' => TRUE]);

    return $config;
  }

  /**
   * Get Unilogin provider.
   *
   * @return \Drupal\ding_unilogin\OAuth2\Client\Provider\StilUniloginProvider
   *   Provider with basic configuration.
   *
   * @throws Exception
   *   If required libraries are not loaded.
   */
  public function getProvider() {
    $configuration = $this->getConfiguration();
    $provider_options = [
      'urlAuthorize' => $configuration['oidc']['authorization_endpoint'] ?? NULL,
      'urlAccessToken' => $configuration['oidc']['token_endpoint'] ?? NULL,
      'urlResourceOwnerDetails' => $configuration['oidc']['userinfo_endpoint'] ?? NULL,
      'urlTokenIntrospect' => $configuration['oidc']['token_introspection_endpoint'] ?? NULL,
      'urlEndSession' => $configuration['oidc']['end_session_endpoint'] ?? NULL,
      'redirectUri' => $configuration['redirect_uri'],
      'clientId' => $configuration['client_id'],
      'clientSecret' => $configuration['client_secret'],
    ];

    return new StilUniloginProvider($provider_options);
  }

}
