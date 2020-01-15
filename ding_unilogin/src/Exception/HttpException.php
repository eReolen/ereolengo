<?php

namespace Drupal\ding_unilogin\Exception;

use RuntimeException;

/**
 * Class HttpException.
 *
 * @package Drupal\ding_unilogin\Exception
 */
abstract class HttpException extends RuntimeException {

  /**
   * Get http status header including status code.
   */
  public function getStatusHeader() {
    return $this->code . ' ' . $this->getStatusHeaderName();
  }

  /**
   * Get http status header name.
   */
  abstract public function getStatusHeaderName();

}
