<?php

namespace Drupal\ding_unilogin\OAuth2\Client\Provider;

use League\OAuth2\Client\Provider\GenericProvider;
use League\OAuth2\Client\Token\AccessToken;

/**
 * Stil Unilogin provider.
 *
 * @see https://viden.stil.dk/display/OFFSKOLELOGIN/OIDC
 */
class StilUniloginProvider extends GenericProvider {

  /**
   * The token introspect URL.
   *
   * @var string
   */
  protected $urlTokenIntrospect;

  /**
   * The end session URL.
   *
   * @var string
   */
  protected $urlEndSession;

  /**
   * {@inheritdoc}
   */
  protected function getRequiredOptions() {
    return array_merge(
      parent::getRequiredOptions(),
      [
        'urlTokenIntrospect',
        'urlEndSession',
      ]
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function getConfigurableOptions() {
    return array_merge(
      parent::getConfigurableOptions(),
      [
        'urlTokenIntrospect',
        'urlEndSession',
      ]
    );
  }

  /**
   * Introspect token, i.e. get user info.
   *
   * @param \League\OAuth2\Client\Token\AccessToken $accessToken
   *   The access token.
   *
   * @return array
   *   The user info.
   */
  public function introspectToken(AccessToken $accessToken): array {
    $request = $this->getRequest('POST',
      $this->urlTokenIntrospect,
      [
        'headers' => [
          'content-type' => 'application/x-www-form-urlencoded',
        ],
        'body' => http_build_query([
          'token' => $accessToken->getToken(),
          'client_id' => $this->clientId,
          'client_secret' => $this->clientSecret,
        ]),
      ]
    );
    $response = $this->getResponse($request);

    return drupal_json_decode($response->getBody()->getContents());
  }

  /**
   * Logout.
   *
   * @see https://auth0.com/docs/authenticate/login/logout/log-users-out-of-auth0
   * @see https://viden.stil.dk/display/OFFSKOLELOGIN/Implementering+af+tjeneste#Implementeringaftjeneste-1.5LogudmedOIDC
   */
  public function logout(string $postLogoutRedirectUri, string $idTokenHint) {
    $request = $this->getRequest('GET',
      $this->urlEndSession,
      [
        'headers' => [
          'content-type' => 'application/x-www-form-urlencoded',
        ],
        'body' => http_build_query([
          'id_token_hint' => $idTokenHint,
          'post_logout_redirect_uri' => $postLogoutRedirectUri,
        ]),
      ]
    );

    return $this->getResponse($request);
  }

}
