<?php

namespace Drupal\ding_unilogin\Exception;

/**
 * Http Not Found Exception class.
 *
 * @package Drupal\ding_unilogin\Exception
 */
class HttpNotFoundException extends HttpException {

  /**
   * {@inheritdoc}
   */
  public function __construct($message = 'Not found', $code = 404, Throwable $previous = NULL) {
    parent::__construct($message, $code, $previous);
  }

  /**
   * {@inheritdoc}
   */
  public function getStatusHeaderName() {
    return 'Bad Request';
  }

}
