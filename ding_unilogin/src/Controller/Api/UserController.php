<?php

namespace Drupal\ding_unilogin\Controller\Api;

use Drupal\ding_unilogin\Exception\HttpBadRequestException;
use Drupal\ding_unilogin\Exception\HttpRuntimeException;

/**
 * User controller.
 */
class UserController extends ApiController {

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
    $this->checkAuthorization($path);

    if (1 === count($path)) {
      return $this->read($path[0]);
    }

    throw new HttpBadRequestException('Invalid request');
  }

  /**
   * Get an user by id.
   *
   * @param string $username
   *   The user id.
   *
   * @return array
   *   The user if found.
   *
   * @throws \Drupal\ding_unilogin\Exception\HttpException
   */
  public function read($username) {
    try {
      $data = _ding_unilogin_get_data($username);
      return ['data' => $data];
    }
    catch (\Exception $exception) {
      throw new HttpRuntimeException('Error processing request', 400, $exception);
    }
  }

}
