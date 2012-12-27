<?php
/**
 * @file
 * The Rules module implementation
 */


/**
 * Implements hook_rules_action_info().
 */
function notificare_rules_action_info() {

  $defaults = array(
   'parameter' => array(
       'node' => array(
         'type' => 'node',
         'label' => t('Node'),
          ),
        'account' => array(
          'type' => 'user',
          'label' => t('User'),
          'optional' => TRUE,
        ),
        'comment' => array(
          'type' => 'comment',
          'label' => t('Comment'),
          'optional' => TRUE,
        ),
    ),
    'group' => t('Notificare'),
  );

  $actions['notificare_notify'] = $defaults + array(
    'label' => t('Send notification through Notificare Service'),
  );


  return $actions;

}

/**
 * Send the info to Notificare
 */
function notificare_notify($node, $account = NULL, $comment = NULL) {
  global $base_url;
  $site_name =  variable_get('site_name', 'Drupal');

  $message = $comment->comment_body[$comment->language][0]['value'];
  $approve_url = $base_url . '/' . $comment->cid . '/edit';
  
  $payload = array(
      "message" => t('[@site_name] Message' , array('@site_name' => $site_name)),
      "fullMessage" => $message,
      "targets" => array(
        array(
          "action" => "open web page",
          "url" => $approve_url, 
          "type" => "url",
          "message" => "false",
          ),
        ),
      );

  $result = send_notificare_message($payload);
}


/**
 * Use drupal_http_request to send message to Notificare.
 */
function send_notificare_message($payload=array()) {


  $data = json_encode($payload);
  $options = array(
    'method' => 'POST',
    'timeout' => 15,
    'data' => http_build_query($payload),
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
    );

  $post_url = variable_get('notificare_post_uri', 'https://apps.notifica.re') . '/' . variable_get('notificare_service', 'hooks/webhook') . '/' . variable_get('notificare_service_token', '') . '?token=' . variable_get('notificare_user_token', '');
  return drupal_http_request($post_url, $options);
}
