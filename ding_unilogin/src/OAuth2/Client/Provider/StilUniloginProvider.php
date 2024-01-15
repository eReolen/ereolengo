<?php

namespace Drupal\ding_unilogin\OAuth2\Client\Provider;

use Drupal\ding_unilogin\Exception\StilUniloginException;
use GuzzleHttp\Exception\ConnectException;
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
   *
   * @see https://viden.stil.dk/display/OFFSKOLELOGIN/Implementering+af+tjeneste#Implementeringaftjeneste-1.3.3ModtagbrugerinformationermedAccessToken
   */
  public function introspectToken(AccessToken $accessToken): array {
    $request = $this->getRequest('POST',
      $this->urlTokenIntrospect,
      [
        'headers' => [
          'content-type' => 'application/x-www-form-urlencoded',
        ],
        'body' => $this->buildQueryString([
          'token' => $accessToken->getToken(),
          'client_id' => $this->clientId,
          'client_secret' => $this->clientSecret,
        ]),
      ]
    );
    $response = $this->getResponse($request);

    try {
      $content = $response->getBody()->getContents();
      return json_decode($content, TRUE, 512, JSON_THROW_ON_ERROR);
    } catch (JsonException $jsonException) {
      throw new StilUniloginException('Cannot parse token', $jsonException->getCode(), $jsonException);
    } catch (\Exception $exception) {
      throw new StilUniloginException('Cannot introspect token', $exception->getCode(), $exception);
    }
  }

  /**
   * Logout.
   *
   * @see https://viden.stil.dk/display/OFFSKOLELOGIN/Implementering+af+tjeneste#Implementeringaftjeneste-1.5LogudmedOIDC
   */
  public function logout(string $postLogoutRedirectUri, string $idTokenHint) {
    $parameters = [
      'id_token_hint' => $idTokenHint,
      'post_logout_redirect_uri' => $postLogoutRedirectUri,
    ];

    // "Logud håndteres af Unilogin brokeren, og kræver derfor kun et get request med redirectUri som parameter. Der arbejdes på at kunne understøtte Frontchannel og Backchannel Logout." (cf. https://viden.stil.dk/display/OFFSKOLELOGIN/Implementering+af+tjeneste#Implementeringaftjeneste-1.5LogudmedOIDC_
    // drupal_goto(url($this->urlEndSession, ['query' => $parameters]));
    // Send the GET request and swallow select exceptions.
    try {
      $request = $this->getRequest('GET', url($this->urlEndSession, ['query' => $parameters]));
      $this->getResponse($request);
    }
    catch (ConnectException $connectException) {
      // ConnectException may be thrown during local development if the redirect
      // URI cannot be accessed from PHP.
    }

    /*
     * This may be a better way to end the session, but it's currently not
     * supported by Stil Unilogin (cf.
     * https://auth0.com/docs/authenticate/login/logout/log-users-out-of-auth0)
     *
     * ```
     * $request = $this->getRequest('POST',
     *   $this->urlEndSession,
     *   [
     *     'headers' => [
     *       'content-type' => 'application/x-www-form-urlencoded',
     *     ],
     *     'body' => $this->buildQueryString($parameters),
     *   ]
     * );
     * $response = $this->getResponse($request);
     * ```
     */
  }

}
