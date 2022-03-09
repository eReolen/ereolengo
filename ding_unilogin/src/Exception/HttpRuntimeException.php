<?php

namespace Drupal\ding_unilogin\Exception;

/**
 * Class HttpBadRequestException.
 *
 * @package Drupal\ding_unilogin\Exception
 */
class HttpRuntimeException extends HttpException {

  /**
   * {@inheritdoc}
   */
  public function __construct($message = 'Runtime exception', $code = 400, \Throwable $previous = NULL) {
    parent::__construct($message, $code, $previous);
  }

  /**
   * {@inheritdoc}
   */
  public function getStatusHeaderName() {
    return 'Runtime exception';
  }

}
