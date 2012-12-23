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
  
 
  
  $user = isset($account->name) ? $account->name : $comment->name;
  $comment_title = isset($comment->subject) ? $comment->subject : '';
  $comment_body = truncate_content($comment->comment_body[$comment->language][0]['value']);
  $node_url = $base_url . '/node/' . $node->nid;
  $approve_url = $base_url . '/' . $comment->cid . '/edit';
  
  $payload = array(
    'hook' => 'comment_post',
    'post_title' => $comment_title,
    'comment_content' => $comment_body,
    'comment_author' => $user,
    'post_url' => $node_url,
    'approve_url' => $approve_url,
    'spam_url' => $approve_url,
    'delete_url' => $approve_url,
    'trash_url' => $approve_url,
  );

  $result = send_notificare_message($payload);
  //dsm($result);
}


/**
 * Use drupal_http_request to send message to Notificare.
 */
function send_notificare_message($payload=array()) {
  $options = array(
                  'method' => 'POST',
                  'body' => $payload,
                  'timeout' => 15,
                  'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
          );
  $post_url = variable_get('notificare_post_uri', 'https://apps.notifica.re') . '/' . variable_get('notificare_service', 'hooks/wordpress') . '/' . variable_get('notificare_service_token', '') . '?token=' . variable_get('notificare_user_token', '');
  return drupal_http_request($post_url, $options);
}

/**
 * Utility function to truncate content
 */
function truncate_content( $content ) {
  if ( strlen( $content ) > 140 ) {
    $content = substr( $content, 0, 140 - 3 ) . '...'; 
  }
  return $content;
}
