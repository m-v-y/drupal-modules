<?php

/**
 * Module settings form.
 *
 * @param $form
 * @param $form_state
 * @return array
 */
function views_style_autoload_settings($form, $form_state) {
  // Show debug information.
  $show_debug_information = variable_get('views_style_autoload_settings_debug', 0);
  $form['show_debug'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display which views used on current page.'),
    '#default_value' => $show_debug_information,
  );

  // Custom files path.
  $files_path = variable_get('views_style_autoload_files_path', '');
  if (empty($files_path)) {
    $default_theme = variable_get('theme_default', '');
    $theme_path = drupal_get_path('theme', $default_theme);
    $files_path = $theme_path . '/css/views-style-autoload';
  }
  $form['files_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Styles path'),
    '#default_value' => $files_path,
  );

  // List of excluded views.
  $excluded_views = variable_get('views_style_autoload_settings_excl_views', '');
  if (!empty($excluded_views)) {
    $excluded_views = implode("\r\n", $excluded_views);
  }
  $form['excluded_views'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Excluded views'),
    '#description'   => 'Input excluded views names. One per line.',
    '#default_value' => $excluded_views,
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for module settings form.
 *
 * @see views_style_autoload_settings().
 *
 * @param $form
 * @param $form_state
 */
function views_style_autoload_settings_submit($form, $form_state) {
  if (isset($form_state['values']['show_debug'])) {
    variable_set('views_style_autoload_settings_debug', $form_state['values']['show_debug']);
  }

  if (!empty($form_state['values']['files_path'])) {
    variable_set('views_style_autoload_files_path', $form_state['values']['files_path']);
  }

  if (!empty($form_state['values']['excluded_views'])) {
    $excluede_views = $form_state['values']['excluded_views'];
    $excluede_views = filter_xss($excluede_views);
    $excluede_views = explode("\r\n", $excluede_views);
    variable_set('views_style_autoload_settings_excl_views', $excluede_views);
  }
}

/**
 * Settings page for style autoload form.
 *
 * @param $object_type
 * @param $object_value
 * @param null $language
 * @return bool|string
 */
function views_style_autoload_view_page($object_type, $object_value, $language = NULL) {
  $form = drupal_get_form('views_style_autoload_view_form', array('view' => $object_value));

  if (empty($form)) {
    drupal_not_found();
  }

  $output = render($form);
  if (empty($output)) {
    drupal_not_found();
  }

  return $output;
}

/**
 * Views autoload form.
 */
function views_style_autoload_view_form($form, $form_state) {
  if (empty($form_state['build_info']['args'][0]['view'])) {
    drupal_not_found();
  }

  $view = $form_state['build_info']['args'][0]['view'];
  if (empty($view->name)) {
    drupal_not_found();
  }

  $form['view_name'] = array(
    '#type'  => 'hidden',
    '#value' => $view->name,
  );

  $used_views = variable_get('views_style_autoload_used_views', array());

  $form['is_active'] = array(
    '#title'         => t('Use style autoload'),
    '#type'          => 'checkbox',
    '#default_value' => isset($used_views[$view->name]) ? 1 : 0,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save settings'));

  return $form;
}

/**
 * Submit handler for view autoload form
 *
 * @see views_style_autoload_view_form().
 *
 * @param $form
 * @param $form_state
 */
function views_style_autoload_view_form_submit($form, $form_state) {
  $values = $form_state['values'];

  if (isset($values['is_active'])) {
    $used_views = variable_get('views_style_autoload_used_views', array());
    if ($values['is_active'] == 1) {
      $used_views[$values['view_name']] = 1;
    }
    else {
      unset($used_views[$values['view_name']]);
    }
    variable_set('views_style_autoload_used_views', $used_views);
  }
}