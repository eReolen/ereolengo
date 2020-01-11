<?php

namespace Drupal\ding_unilogin;

use Drupal\ding_unilogin\Controller\InstitutionController;

/**
 * Router for api requests.
 */
class ApiRouter {

  /**
   * Route to controller.
   */
  public function route(array $args) {
    $controllerName = array_shift($args);

    $controller = $this->getController($controllerName);

    return $controller->handle($args);
  }

  /**
   * Get a controller by name.
   */
  private function getController($name) {
    switch ($name) {
      case 'institutions':
        return new InstitutionController();
    }

    throw new \RuntimeException(sprintf('Invalid controller name: %s', $name));
  }

}
