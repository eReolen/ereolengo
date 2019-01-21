<?php

/**
 * @file
 * Page callbacks for fancy box.
 */

/**
 * News page callback.
 */
function breol_news_page_news_page() {
  $query_parameters = drupal_get_query_parameters();
  $items = 30;

  // Define title.
  $title = t('News', array(), array('context' => 'pluralis'));

  if (empty($query_parameters)) {
    // Define query.
    $news_query = _breol_news_page_news_query_empty($items);
  }
  else {
    $news_category = '';
    foreach ($query_parameters as $key => $value) {
      if ($key == 'news_category') {
        $news_category = $value;
      }
    }
    // Change title.
    $term = taxonomy_term_load($news_category);
    $title = !empty($term) ? t('News about !term', array('!term' => $term->name), array('context' => 'pluralis')) : $title;

    // Define query.
    $news_query = _breol_news_page_news_query_parameters($items, $news_category);
  }

  $news_res = $news_query->execute();
  $build = array();

  if (isset($news_res['node'])) {
    $nodes = node_load_multiple(array_keys($news_res['node']));
    $build = node_view_multiple($nodes, 'teaser');
  }

  $image_file = '';
  if ($fid = variable_get('breol_news_page_image', NULL)) {
    $image_file = file_load($fid);
  }

  // Set page title.
  drupal_set_title($title);

  return array(
    '#theme' => 'breol_news_page',
    '#title' => $title,
    '#content' => $build,
    '#image_file' => $image_file,
    '#pager' => array(
      '#theme' => 'pager',
      '#element' => 1,
    ),
  );
}

/**
 * Implements hook_query_TAG_alter()
 *
 * Get all relevant nodes without a news type tag.
 */
function breol_news_page_query_node_is_not_tagged_alter(QueryAlterableInterface $query) {
  $query->leftJoin('field_data_field_news_type', 'o', '(node.nid = o.entity_id AND o.entity_type = :entity_type)', array(':entity_type' => 'node'));
  $query->isNull('o.field_news_type_tid');
}

/**
 * Get all items without a tag.
 *
 * See https://www.drupal.org/project/drupal/issues/1157006#comment-7634027
 *
 * @param $items integer
 *   The number of items to display.
 *
 * @return \EntityFieldQuery
 *   The entity field query.
 */
function _breol_news_page_news_query_empty($items) {
  $news_query = new EntityFieldQuery();
  return $news_query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'breol_news')
    ->addTag('node_is_not_tagged')
    ->propertyCondition('status', 1)
    ->propertyOrderBy('changed', 'DESC')
    ->pager($items, 1);
}

/**
 * Get all items with a certain tag.
 *
 * @param $items integer
 *   The number of items to display.
 * @param $news_category string.
 *   The query parameters.
 *
 * @return \EntityFieldQuery
 *  The entity field query
 */
function _breol_news_page_news_query_parameters($items, $news_category) {
  // Make entity field query with news catagory as condition.
  $news_query = new EntityFieldQuery();
  return $news_query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'breol_news')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_news_type', 'tid', array($news_category))
    ->propertyOrderBy('changed', 'DESC')
    ->pager($items, 1);
}