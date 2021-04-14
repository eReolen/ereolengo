<?php

namespace Drupal\ding_unilogin\Exception;

/**
 * Class HttpBadRequestException.
 *
 * @package Drupal\ding_unilogin\Exception
 */
class HttpBadRequestException extends HttpException {

  /**
   * {@inheritdoc}
   */
  public function __construct($message = 'Bad request', $code = 400, Throwable $previous = NULL) {
    parent::__construct($message, $code, $previous);
  }

  /**
   * {@inheritdoc}
   */
  public function getStatusHeaderName() {
    return 'Bad Request';
  }

}
