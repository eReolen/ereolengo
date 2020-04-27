<?php

namespace Drupal\ding_unilogin\Controller;

use Drupal\ding_unilogin\Exception\HttpBadRequestException;
use Drupal\ding_unilogin\Exception\HttpNotFoundException;
use Drupal\ding_unilogin\Exception\HttpUnauthorizedException;

/**
 * API controller.
 */
abstract class ApiController {

  /**
   * Handle request.
   *
   * @param array $path
   *   The request path.
   *
   * @return array|null
   *   The data.
   *
   * @throws \Drupal\ding_unilogin\Exception\HttpException
   */
  abstract public function handle(array $path);

  /**
   * Check that user is authorized to use the api as requested.
   *
   * @param array $path
   *   The path.
   *
   * @throws \Drupal\ding_unilogin\Exception\HttpUnauthorizedException
   *   If user is not authorized.
   */
protected function checkAuthorization(array $path) {
    if (user_access('configure unilogin')) {
      return;
    }

    $authorization = $this->getRequestHeader('authorization');
    if (!preg_match('/^(bearer|token) (?P<token>.+)$/i', $authorization, $matches)) {
      throw new HttpUnauthorizedException();
    }
    $token = $matches['token'];
    $method = $_SERVER['REQUEST_METHOD'];

    switch ($method) {
      case 'POST':
        if (variable_get('ding_unilogin_api_token_write') !== $token) {
          throw new HttpUnauthorizedException();
        }
        break;

      case 'GET':
        if (variable_get('ding_unilogin_api_token_read') !== $token) {
          throw new HttpUnauthorizedException();
        }
        break;
    }

    // Authorized with token.
  }

  /**
   * Get a http request header by name.
   *
   * @param string $name
   *   The request header name.
   *
   * @return string|null
   *   The request header if found.
   */
  protected function getRequestHeader($name) {
    $name = strtoupper(preg_replace('/[^a-z0-9]/i', '_', $name));

    // Workaround for authorization header (which is removed by Varnish).
    if ('AUTHORIZATION' === $name && isset($_SERVER['HTTP_X_' . $name])) {
      return $_SERVER['HTTP_X_' . $name];
    }

    return $_SERVER['HTTP_' . $name] ?? $_SERVER[$name] ?? NULL;
  }

}
