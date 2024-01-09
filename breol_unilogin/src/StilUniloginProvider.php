<?php

namespace Drupal\breol_unilogin;

use League\OAuth2\Client\Provider\GenericProvider;
use League\OAuth2\Client\Token\AccessToken;

/**
 * Stil Unilogin provider.
 */
class StilUniloginProvider extends GenericProvider {

  /**
   * The token introspect URL.
   *
   * @var string
   */
  protected $urlTokenIntrospect;

  /**
   * {@inheritdoc}
   */
  protected function getRequiredOptions() {
    return array_merge(
      parent::getRequiredOptions(),
      [
        'urlTokenIntrospect',
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

}
