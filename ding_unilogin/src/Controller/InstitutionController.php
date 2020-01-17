<?php

namespace Drupal\ding_unilogin\Controller;

use Drupal\ding_unilogin\Exception\HttpBadRequestException;
use Drupal\ding_unilogin\Exception\HttpNotFoundException;
use Drupal\ding_unilogin\Exception\HttpUnauthorizedException;

/**
 * Institution controller.
 */
class InstitutionController {

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
  public function handle(array $path) {
    $authorization = $this->getRequestHeader('authorization');
    if (!preg_match('/^(bearer|token) (?P<token>.+)$/i', $authorization, $matches)) {
      throw new HttpUnauthorizedException();
    }
    $token = $matches['token'];

    if (empty($path)) {
      $method = $_SERVER['REQUEST_METHOD'];

      switch ($method) {
        case 'POST':
          if (variable_get('ding_unilogin_api_token_write') !== $token) {
            throw new HttpUnauthorizedException();
          }
          return $this->update();

        case 'GET':
          if (variable_get('ding_unilogin_api_token_read') !== $token) {
            throw new HttpUnauthorizedException();
          }
          return $this->list();

        default:
          throw new HttpBadRequestException(sprintf('Method %s not supported', $method));
      }
    }
    elseif (1 === count($path)) {
      return $this->read($path[0]);
    }

    throw new HttpBadRequestException('Invalid request');
  }

  /**
   * List of institutions.
   *
   * @return array
   *   The list of institutions.
   */
  public function list() {
    $institutions = _ding_unilogin_get_institutions();

    return ['data' => $institutions];
  }

  /**
   * Get an institution by id.
   *
   * @param string $id
   *   The institution id.
   *
   * @return array
   *   The institution if found.
   *
   * @throws \Drupal\ding_unilogin\Exception\HttpException
   */
  public function read($id) {
    $institutions = _ding_unilogin_get_institutions();

    if (isset($institutions[$id])) {
      return ['data' => $institutions[$id]];
    }

    throw new HttpNotFoundException(sprintf('Invalid institution id: %s', $id));
  }

  /**
   * Update the list of institutions.
   *
   * @throws \Drupal\ding_unilogin\Exception\HttpException
   */
  public function update() {
    $contentType = $this->getRequestHeader('content-type');
    if ('application/vnd.api+json' !== $contentType) {
      throw new HttpBadRequestException(sprintf('Invalid content type: %s', $contentType));
    }

    $payload = json_decode(file_get_contents('php://input'), TRUE);
    if (empty($payload) || empty($payload['data'])) {
      throw new HttpBadRequestException(sprintf('Invalid content'));
    }

    $data = $payload['data'];

    // Validate data.
    foreach ($data as $id => &$item) {
      if (!isset($item['name'], $item['group'], $item['type'], $item['number_of_members'])) {
        throw new HttpBadRequestException(sprintf('Invalid content'));
      }
      if (!preg_match('/^[a-z0-9]{6}$/i', (string) $id)) {
        throw new HttpBadRequestException(sprintf('Invalid id: %s', $id));
      }
      if (!is_int($item['number_of_members']) || $item['number_of_members'] <= 0) {
        throw new HttpBadRequestException(sprintf('Invalid number_of_members: %d', $item['number_of_members']));
      }

      $item['id'] = $id;
    }

    _ding_unilogin_set_institutions($data);

    return NULL;
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
