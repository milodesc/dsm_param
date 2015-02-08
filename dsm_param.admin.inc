<?php

/**
 * @file
 * Administration page callbacks for the Parameter Message module.
 */

/**
 * Returns an administrative page for the Parameter Message module.
 *
 * Existing parameter messages can be edited or deleted and new parameter
 * messages can be created.
 *
 * @return string
 *   An HTML-formatted string with the administrative page content.
 *
 * @see dsm_param_menu()
 */
function dsm_param_admin() {
  $header = array(
    t('Parameter Name'),
    t('Message Status'),
    t('Repeat Message'),
    t('Edit / Delete'),
  );
  $rows = array();
  $result = db_query('select * from {dsm_param_parameters}');
  foreach ($result as $parameter) {
    $rows[] = array(
      $parameter->parameter_name,
      $parameter->message_status,
      $parameter->repeat_message,
      l(t('Edit'), 'admin/config/dsm_param/' . $parameter->pid . '/edit') . ' | ' . l(t('Delete'), 'admin/config/dsm_param/' . $parameter->pid . '/delete'),
    );
  }
  if (!empty($rows)) {
    $output = theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('dsm_param_config')),
    ));
    $output .= l(t('Create a new parameter'), 'admin/config/dsm_param/create');
  }
  else {
    $output = t("There aren't any parameters currently set up.") . " " . l(t('Click here to set one up.'), 'admin/config/dsm_param/create');
  }
  return $output;
}

/**
 * Returns an administrative form for creating a Parameter Message.
 *
 * @return string
 *   An HTML-formatted string with the administrative page content.
 *
 * @see dsm_param_menu()
 */
function dsm_param_create_parameter() {
  $form = dsm_param_get_param_form();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Create Parameter',
  );
  return $form;
}

/**
 * Validate dsm_param_create_parameter form submissions.
 *
 * @see dsm_param_create_parameter()
 * @see dsm_param_create_parameter_submit()
 * @see dsm_param_validate_param_form()
 */
function dsm_param_create_parameter_validate($form, &$form_state) {
  dsm_param_validate_param_form($form);
}

/**
 * Process dsm_param_create_parameter form submissions.
 *
 * Adds new parameters to the database.
 *
 * @see dsm_param_create_parameter()
 * @see dsm_param_create_parameter_validate()
 */
function dsm_param_create_parameter_submit($form, &$form_state) {
  db_insert('dsm_param_parameters')
    ->fields(array(
      'parameter_name' => $form['dsm_param_parameter_name']['#value'],
      'message_status' => $form['dsm_param_message_status']['#value'],
      'repeat_message' => $form['dsm_param_message_repeat']['#value'],
    ))
    ->execute();
  drupal_set_message(t('The @param parameter has been saved.', array('@param' => $form['dsm_param_parameter_name']['#value'])));
  drupal_goto('admin/config/dsm_param');
}


/**
 * Returns an administrative form for editing a Parameter Message.
 *
 * @return string
 *   An HTML-formatted string with the edit page content.
 *
 * @see dsm_param_menu()
 */
function dsm_param_edit_parameter($form, $form_state, $pid) {
  $parameter_message = dsm_param_get_parameter($pid);
  $form = dsm_param_get_param_form();
  $form['dsm_param_parameter_name']['#default_value'] = $parameter_message->parameter_name;
  $form['dsm_param_message_status']['#default_value'] = $parameter_message->message_status;
  $form['dsm_param_message_repeat']['#default_value'] = $parameter_message->repeat_message;
  $form['dsm_param_pid'] = array(
    '#value' => $parameter_message->pid,
    '#type' => 'hidden',
    '#access' => FALSE,
  );
  $form['edit_flag'] = array(
    '#type' => 'hidden',
    '#value' => 1,
    '#access' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Parameter',
  );
  return $form;
}

/**
 * Validate dsm_param_edit_parameter form submissions.
 *
 * @see dsm_param_edit_parameter()
 * @see dsm_param_edit_parameter_submit()
 * @see dsm_param_validate_param_form()
 */
function dsm_param_edit_parameter_validate($form, &$form_state) {
  dsm_param_validate_param_form($form);
}

/**
 * Process dsm_param_edit_parameter form submissions.
 *
 * Updates existing parameters with new values.
 *
 * @see dsm_param_edit_parameter()
 * @see dsm_param_edit_parameter_validate()
 */
function dsm_param_edit_parameter_submit($form, &$form_state) {
  $pid = $form['dsm_param_pid']['#value'];
  db_update('dsm_param_parameters')
    ->fields(array(
      'parameter_name' => $form['dsm_param_parameter_name']['#value'],
      'message_status' => $form['dsm_param_message_status']['#value'],
      'repeat_message' => $form['dsm_param_message_repeat']['#value'],
    ))
    ->condition('pid', $pid)
    ->execute();
  drupal_set_message(t('The @param parameter has been saved.', array('@param' => $form['dsm_param_parameter_name']['#value'])));
  drupal_goto('admin/config/dsm_param');
}

/**
 * Delete a Parameter Message.
 *
 * @see dsm_param_menu()
 */
function dsm_param_delete_parameter($pid) {
  $result = db_query("select * from {dsm_param_parameters} where pid = :pid", array(':pid' => $pid));
  $parameter_message = $result->fetchObject();
  db_delete('dsm_param_parameters')
    ->condition('pid', $parameter_message->pid)
    ->execute();
  drupal_set_message(t('Deleted the @param parameter.', array('@param', $parameter_message->parameter_name)));
  drupal_goto('admin/config/dsm_param');
}

/**
 * Provides form fields needed for creating and editing Parameter messages.
 *
 * @return array
 *   An HTML-formatted string with the edit page content.
 */
function dsm_param_get_param_form() {
  $form = array();
  $form['dsm_param_parameter_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Parameter Name'),
    '#size' => 20,
    '#maxlength' => 64,
    '#description' => t("If a parameter with this name is in the URL, drupal will set a message for the user with the parameter's value."),
    '#required' => TRUE,
  );
  $form['dsm_param_message_status'] = array(
    '#type' => 'select',
    '#title' => t('Message Status'),
    '#default_value' => 'status',
    '#description' => t("The message's type, either status, warning or error."),
    '#options' => array(
      'status' => t('Status'),
      'warning' => t('Warning'),
      'error' => t('Error'),
    ),
    '#required' => TRUE,
  );
  $form['dsm_param_message_repeat'] = array(
    '#type' => 'radios',
    '#title' => ('Repeat Message'),
    '#default_value' => 'FALSE',
    '#description' => t("If this is set to False and the message has already been set, then the message won't be repeated."),
    '#options' => array(
      'TRUE' => t('True'),
      'FALSE' => t('False'),
    ),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Provides form validation for creating and editing parameter messages.
 *
 * @param array $form
 *   The create or edit parameter message form.
 */
function dsm_param_validate_param_form($form) {
  $parameter_name = $form['dsm_param_parameter_name']['#value'];

  if (preg_match('/[^a-z_\-0-9]/i', $parameter_name)) {
    form_set_error('dsm_param_parameter_name', 'The Parameter Name supplied contains illegal characters. Please use only letters, numbers, underscores and dashes.');
  }

  // Don't let admins create a parameter called 'q', since that is used on every
  // page request in Drupal.
  if ($parameter_name == 'q') {
    form_set_error('dsm_param_parameter_name', "'q' is an invalid name for a parameter. Please choose another name.");
  }

  if (!empty($form['edit_flag']) && $form['edit_flag']['#value'] != 1) {
    // Check if name supplied is already used.
    $result = db_query('select parameter_name from {dsm_param_parameters}');
    foreach ($result as $parameter) {
      if ($parameter_name == $parameter->parameter_name) {
        form_set_error('dsm_param_parameter_name', 'The parameter name is already in use. Please choose another name.');
      }
    }
  }

  // Check the validity of the Message Status field.
  $valid_statuses = array('status', 'warning', 'error');
  $submitted_status = $form['dsm_param_message_status']['#value'];
  if (!in_array($submitted_status, $valid_statuses)) {
    form_set_error('dsm_param_message_status', 'You have submitted an invalid message status. Please choose another status.');
  }

  // Check the validity of the Repeat Message field.
  $valid_repeat = array('TRUE', 'FALSE');
  $submitted_repeat = $form['dsm_param_message_repeat']['#value'];
  if (!in_array($submitted_repeat, $valid_repeat)) {
    form_set_error('dsm_param_message_repeat', 'You have submitted an invalid value for the Repeat Message field. Please choose true or false.');
  }
}

/**
 * Get a Parameter Message object based on a pid.
 *
 * @param int $pid
 *   pid of the Parameter Message to be returned.
 *
 * @return mixed
 *   The Parameter Message object with the specified pid.
 */
function dsm_param_get_parameter($pid) {
  $result = db_query("select * from {dsm_param_parameters} where pid = :pid", array(':pid' => $pid));
  return $result->fetchObject();
}
