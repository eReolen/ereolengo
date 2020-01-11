<?php

namespace Drupal\ding_unilogin\Exception;

/**
 * Class HttpException.
 *
 * @package Drupal\ding_unilogin\Exception
 */
abstract class HttpException extends \RuntimeException {

  /**
   * Get http status header including status code.
   */
  public function getStatusHeader() {
    return $this->code . ' ' . $this->getStatusHeaderName();
  }

  /**
   * Get http status header name.
   */
  public function getStatusHeaderName() {
    switch ($this->code) {
      case 401:
        return 'Unauthorized';

      default:
        return 'Bad Request';
    }
  }

}
