<?php
/**
 * @file
 * breol_news.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function breol_news_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function breol_news_image_default_styles() {
  $styles = array();

  // Exported image style: app_feed_image.
  $styles['app_feed_image'] = array(
    'label' => 'App feed image',
    'effects' => array(
      1 => array(
        'name' => 'manualcrop_crop_and_scale',
        'data' => array(
          'width' => 500,
          'height' => 500,
          'upscale' => 0,
          'respectminimum' => 1,
          'onlyscaleifcrop' => 0,
          'style_name' => 'app_feed_image',
        ),
        'weight' => 0,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function breol_news_node_info() {
  $items = array(
    'breol_news' => array(
      'name' => t('Nyhed'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Titel'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
