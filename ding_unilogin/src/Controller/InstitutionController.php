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
   */
  public function handle(array $args) {
    $authorization = $this->getRequestHeader('authorization');
    if (!preg_match('/^token (?P<token>.+)$/', $authorization, $matches)) {
      throw new HttpUnauthorizedException();
    }
    $token = $matches['token'];

    if (empty($args)) {
      $method = $_SERVER['REQUEST_METHOD'];

      if ('POST' === $method) {
        if (variable_get('ding_unilogin_api_token_write') !== $token) {
          throw new HttpUnauthorizedException();
        }

        return $this->update();
      }

      if (variable_get('ding_unilogin_api_token_read') !== $token) {
        throw new HttpUnauthorizedException();
      }

      return $this->list();
    }
    elseif (1 === \count($args)) {
      return $this->read($args[0]);
    }

    throw new HttpBadRequestException('Invalid request');
  }

  /**
   * List of institutions.
   */
  public function list() {
    $institutions = _ding_unilogin_get_institutions();

    return ['data' => $institutions];
  }

  /**
   * Get an institution by id.
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
      if (!isset($item['name'], $item['group'], $item['type'], $item['number_of_members'])
        || !preg_match('/^[a-z0-9]{6}$/', (string) $id)) {
        throw new HttpBadRequestException(sprintf('Invalid content'));
      }
      $item['id'] = $id;
    }

    _ding_unilogin_set_institutions($data);

    return NULL;
  }

  /**
   * Get a http request header by name.
   */
  protected function getRequestHeader($name) {
    $name = strtoupper(preg_replace('/[^a-z0-9]/i', '_', $name));

    return $_SERVER['HTTP_' . $name] ?? $_SERVER[$name] ?? NULL;
  }

}
