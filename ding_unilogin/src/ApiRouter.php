<?php

namespace Drupal\ding_unilogin;

use Drupal\ding_unilogin\Controller\InstitutionController;
use Drupal\ding_unilogin\Controller\MunicipalityController;
use Drupal\ding_unilogin\Controller\UserController;
use RuntimeException;

/**
 * Router for api requests.
 */
class ApiRouter {

  /**
   * Route to controller.
   *
   * @param array $path
   *   The request path.
   *
   * @return array|null
   *   The data returned from the controller.
   *
   * @throws \Drupal\ding_unilogin\Exception\HttpException
   */
  public function route(array $path) {
    $controllerName = array_shift($path);

    $controller = $this->getController($controllerName);

    return $controller->handle($path);
  }

  /**
   * Get a controller by name.
   *
   * @param string $name
   *   The controller name.
   *
   * @return \Drupal\ding_unilogin\Controller\InstitutionController
   *   The controller if found.
   *
   * @throws \RuntimeException
   */
  private function getController($name) {
    switch ($name) {
      case 'institutions':
        return new InstitutionController();

      case 'municipalities':
        return new MunicipalityController();

      case 'users':
        return new UserController();

      default:
        throw new RuntimeException(sprintf('Invalid controller name: %s', $name));
    }
  }

}
