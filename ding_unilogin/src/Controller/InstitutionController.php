<?php

namespace Drupal\ding_unilogin\Controller;

use Drupal\ding_unilogin\Exception\HttpBadRequestException;

/**
 * Institution controller.
 */
class InstitutionController {

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
    $method = $_SERVER['REQUEST_METHOD'];

    switch ($method) {
      case 'GET':
        return $this->list();

      default:
        throw new HttpBadRequestException(sprintf('Method %s not supported', $method));
    }

    throw new HttpBadRequestException('Invalid request');
  }

  /**
   * List of institutions.
   *
   * @return array
   *   The list of institutions.
   */
  public function list() {
    $format = drupal_get_query_parameters()['_format'] ?? NULL;
    if (!empty($format)) {
      return $this->export($format);
    }

    $settings = [
      'api_url' => url('unilogin/institutions', ['query' => ['_format' => 'json']]),
    ];

    return [
      '#type' => 'container',
      '#prefix' => '<div class="institution-list">',
      '#suffix' => '</div>',

      'header' => [
        '#type' => 'container',
        '#prefix' => '<div class="header">',
        '#suffix' => '</div>',

        'export_csv' => [
          '#markup' => l(t('Export all institutions (CSV)'), 'unilogin/institutions', [
            'query' => ['_format' => 'csv'],
            'attributes' => ['class' => ['export', 'csv']],
          ]),
        ],
      ],
      'app' => [
        '#markup' => '<div id="app"></div>',
      ],

      '#attached' => [
        'css' => [
          drupal_get_path('module', 'ding_unilogin') . '/build/institution/list.css' => [],
        ],
        'js' => [
          [
            'type' => 'setting',
            'data' => ['ding_unilogin' => $settings],
          ],
          [
            'data' => drupal_get_path('module', 'ding_unilogin') . '/build/institution/list.js',
            'type' => 'file',
            'scope' => 'footer',
          ],
        ],
      ],
    ];
  }

  /**
   * Export UNI•Login institutions data.
   *
   * @param string $format
   *   The format.
   */
  private function export($format) {
    // Call the UNI•Login institutions api to get data.
    $apiUrl = url('unilogin/api/institutions', ['absolute' => TRUE]);
    $response = drupal_http_request($apiUrl, [
      'headers' => [
        'x-authorization' => 'token ' . variable_get('ding_unilogin_api_token_read'),
      ],
    ]);
    // Forward status and all headers.
    drupal_add_http_header('Status', $response->code . ' ' . $response->status_message);
    foreach ($response->headers as $name => $value) {
      drupal_add_http_header($name, $value);
    }

    if ('csv' === $format) {
      drupal_add_http_header('content-type', 'text/csv');
      $filename = 'ereolengo-institutions-' . (new \DateTime())->format('Ymd') . '.csv';
      drupal_add_http_header('content-disposition', 'attachment; filename="' . $filename . '"');
      $data = json_decode($response->data);
      $out = fopen('php://output', 'w');
      // Write CSV header.
      fputcsv($out, [
        'skolenavn',
        'institutionsnummer',
        'number_of_members',
        'kommune',
      ]);
      foreach ($data->data as $item) {
        $municipality = $item->municipality;
        fputcsv($out, [
          $item->name,
          $item->id,
          $item->number_of_members,
          // Some municipalities are fake and don't have a name ("kommune").
          $municipality->kommune ?? NULL,
        ]);
      }
      fclose($out);
    }
    else {
      echo $response->data;
    }
    drupal_exit();
  }

}
