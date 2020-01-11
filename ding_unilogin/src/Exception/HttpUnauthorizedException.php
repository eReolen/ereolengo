<?php

namespace Drupal\ding_unilogin\Exception;

/**
 * Class HttpUnauthorizedException.
 *
 * @package Drupal\ding_unilogin\Exception
 */
class HttpUnauthorizedException extends HttpException {

  /**
   * {@inheritdoc}
   */
  public function __construct($message = 'Unauthorized', $code = 401, Throwable $previous = NULL) {
    parent::__construct($message, $code, $previous);
  }

}
