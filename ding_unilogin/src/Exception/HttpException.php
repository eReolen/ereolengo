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
   *
   * @return string
   *   The http status header.
   */
  public function getStatusHeader() {
    return $this->code . ' ' . $this->getStatusHeaderName();
  }

  /**
   * Get http status header name.
   *
   * @return string
   *   The http status header name.
   */
  abstract public function getStatusHeaderName();

}
