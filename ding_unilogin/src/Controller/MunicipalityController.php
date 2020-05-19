<?php

namespace Drupal\ding_unilogin\Controller;

use Drupal\ding_unilogin\Exception\HttpBadRequestException;
use Drupal\ding_unilogin\Exception\HttpNotFoundException;

/**
 * User controller.
 */
class MunicipalityController extends ApiController {

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

    if (empty($path)) {
      $method = $_SERVER['REQUEST_METHOD'];

      switch ($method) {
        case 'GET':
          return $this->list();

        default:
          throw new HttpBadRequestException(sprintf('Method %s not supported', $method));
      }
    }
    elseif (1 === count($path)) {
      return $this->read($path[0]);
    }

    throw new HttpBadRequestException('Invalid request');
  }


  /**
   * List of municipalities.
   *
   * @return array
   *   The list of municipalities.
   */
  public function list() {
    $municipalities = $this->getMunicipalities();

    return ['data' => $municipalities];
  }

  /**
   * Get an municipality by id.
   *
   * @param string $id
   *   The municipality id.
   *
   * @return array
   *   The municipality if found.
   *
   * @throws \Drupal\ding_unilogin\Exception\HttpException
   */
  public function read($id) {
    $municipalities = $this->getMunicipalities();

    if (isset($municipalities[$id])) {
      return ['data' => $municipalities[$id]];
    }

    throw new HttpNotFoundException(sprintf('Invalid municipality id: %s', $id));
  }

  private function getMunicipalities() {
    $municipalities = _ding_unilogin_get_municipalities(TRUE);

    // @TODO: Add Publizon keys.
    $libraries = publizon_get_libraries();
    // Index by kommunenr (unilogin_id).
    $libraries = array_column($libraries, null, 'unilogin_id');
    foreach ($municipalities as &$municipality) {
      $retailer = $libraries[$municipality->kommunenr] ?? NULL;
      if (NULL !== $retailer) {
        $municipality->retailer_id = $retailer->retailer_id;
        $municipality->library_name = $retailer->library_name;
      }
    }

    return $municipalities;
  }

}
