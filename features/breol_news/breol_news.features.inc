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

  // Exported image style: app_news_image.
  $styles['app_news_image'] = array(
    'label' => 'App news image',
    'effects' => array(
      3 => array(
        'name' => 'manualcrop_crop',
        'data' => array(
          'width' => 1080,
          'height' => 400,
          'keepproportions' => 1,
          'reuse_crop_style' => '',
          'style_name' => 'app_news_image',
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: web_news_image.
  $styles['web_news_image'] = array(
    'label' => 'Web news image',
    'effects' => array(
      4 => array(
        'name' => 'manualcrop_crop',
        'data' => array(
          'width' => 880,
          'height' => 495,
          'keepproportions' => 1,
          'reuse_crop_style' => '',
          'style_name' => 'web_news_image',
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
