<?php

/**
 * Module settings form.
 *
 * @param $form
 * @param $form_state
 * @return array
 */
function views_style_autoload_settings($form, &$form_state) {
  $show_debug_information = variable_get('views_style_autoload_settings_debug', 0);
  $form['show_debug'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Display which views used on current page.'),
    '#default_value' => $show_debug_information,
  );

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

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validate handler for module settings form.
 *
 * @see views_style_autoload_settings().
 *
 * @param $form
 * @param $form_state
 */
function views_style_autoload_settings_validate($form, &$form_state) {
  // Validate empty path.
  if (empty($form_state['values']['files_path'])) {
    $message = 'The field "Styles path" must be filled';
    form_set_error('files_path', $message);
  }

  // Validate invalid path
  $is_directory_exist = is_dir($form_state['values']['files_path']);
  if ($is_directory_exist == FALSE) {
    $message = t('The directory @dir not exist', array(
      '@dir' => $form_state['values']['files_path'],
    ));
    form_set_error('files_path', $message);
  }
}

/**
 * Submit handler for module settings form.
 *
 * Save module settings.
 *
 * @see views_style_autoload_settings().
 *
 * @param $form
 * @param $form_state
 */
function views_style_autoload_settings_submit($form, &$form_state) {
  if (isset($form_state['values']['show_debug'])) {
    variable_set('views_style_autoload_settings_debug', $form_state['values']['show_debug']);
  }

  if (!empty($form_state['values']['files_path'])) {
    $form_state['values']['files_path'] = filter_xss($form_state['values']['files_path'], array());
    variable_set('views_style_autoload_files_path', $form_state['values']['files_path']);
  }
}

/**
 * Settings page for style auto load form.
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
 * Views auto load settings form.
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

  $items = array();
  $items[] = t('Base ccs class: .@class', array('@class' => views_style_autoload_prepare_name($view->name)));
  $form['help'] = array(
    '#markup' => theme('item_list', array('items' => $items, 'title' => t('Help'))),
  );

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