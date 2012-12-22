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

  $username = isset($account->name) ? $account->name : t('Anonymous');

  $payload = array(
    'hook' => 'comment_post',
    'post_title' => 'test title',
    'comment_content' => 'comment content',
    'comment_author' => $username,
    'post_url' => 'post url',
    );
  $payload = array(
    'hook' => 'comment_post',
    'post_title' => 'Mijn test comment',
    'comment_content' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit.',
    'comment_author' => 'Peter R. de Vries',
    'post_url' => 'http://test.com/bla',
    'approve_url' => 'http://test.com/bla',
    'spam_url' => 'http://test.com/bla',
    'delete_url' => 'http://test.com/bla',
    'trash_url' => 'http://test.com/bla',
  );

  $result = send_notificare_message($payload);

}