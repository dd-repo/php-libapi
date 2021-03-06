<?php

class user extends ldap
{
	public $schema;
	
	public function __construct()
	{
		$this->schema = array();
		$this->schema['objectClass'] = array();
		$this->schema[] = 'uid';
		$this->schema[] = 'uidNumber';
		$this->schema[] = 'gidNumber';
		$this->schema[] = 'givenName';
		$this->schema[] = 'sn';
		$this->schema[] = 'loginShell';
		$this->schema[] = 'o';
		$this->schema[] = 'l';
		$this->schema[] = 'mail';
		$this->schema[] = 'homeDirectory';
		$this->schema[] = 'cn';
		$this->schema[] = 'userPassword';
		$this->schema[] = 'memberUid';
		$this->schema[] = 'mailDeliveryOption';
		$this->schema[] = 'mailQuota';
		$this->schema[] = 'mailAlternateAddress';
		$this->schema[] = 'mailForwardingAddress';
		$this->schema[] = 'mailHost';
		$this->schema[] = 'inetSubscriberStatus';
		$this->schema[] = 'inetAuthorizedServices';
		$this->schema[] = 'host';
		$this->schema[] = 'preferredLanguage';
		$this->schema[] = 'postalAddress';
		$this->schema[] = 'postalCode';
		$this->schema[] = 'description';
		$this->schema[] = 'gecos';
		$this->schema[] = 'owner';
		$this->schema[] = 'title';
		$this->schema[] = 'sshPublicKey';
		$this->schema['objectClass'][] = 'top';
		$this->schema['objectClass'][] = 'person';
		$this->schema['objectClass'][] = 'inetOrgPerson';
		$this->schema['objectClass'][] = 'shadowAccount';
		$this->schema['objectClass'][] = 'posixAccount';
		$this->schema['objectClass'][] = 'posixGroup';
		$this->schema['objectClass'][] = 'mailRecipient';
		$this->schema['objectClass'][] = 'groupOfNames';
		$this->schema['objectClass'][] = 'inetSubscriber';
		$this->schema['objectClass'][] = 'ldapPublicKey';
		
		return $this->schema;
	}

	public function build($row)
	{
		$data = array();
		$data['uid'] = $row['uid'];
		if( $row['userPassword'] )
			$data['userPassword'] = $row['userPassword'];
		$data['uidNumber'] = $GLOBALS['ldap']->newUID();
		$data['gidNumber'] = $data['uidNumber'];
		$data['givenName'] = ($row['givenName']?"{$row['givenName']}":"{$row['uid']}");
		$data['sn'] = ($row['sn']?"{$row['sn']}":"Unkown");
		$data['loginShell'] = $GLOBALS['CONFIG']['SHELL'];
		$data['mail'] = $row['uid'] . '@' . $row['domain'];
		$data['homeDirectory'] = ldap::dirFromDN($row['dn']);
		if( $row['mailForwardingAddress'] )
			$data['mailForwardingAddress'] = $row['mailForwardingAddress'];
		if( $row['gecos'] )
			$data['gecos'] = $row['gecos'];
		$data['cn'] = $row['uid'];
		$data['mailQuota'] = '1G';
		$data['mailDeliveryOption'][] = 'mailbox';
		$data['mailDeliveryOption'][] = 'forward';
		$data['inetSubscriberStatus'] = 'active';
		$data['inetAuthorizedServices'][] = 'imap';
		$data['inetAuthorizedServices'][] = 'imaps';
		$data['inetAuthorizedServices'][] = 'pop3';
		$data['inetAuthorizedServices'][] = 'pop3s';
		$data['inetAuthorizedServices'][] = 'smtp';
		$data['inetAuthorizedServices'][] = 'smtps';
		$data['objectClass'] = $this->schema['objectClass'];
		
		if( $row['ipHostNumber'] )
        {
            $data['objectClass'][] = 'ipHost';
            $data['ipHostNumber'] = $row['ipHostNumber'];
            $data['inetAuthorizedServices'][] = 'sshd';
			$data['host'][] = 'proxy-001';
			$data['host'][] = 'proxy-002';
			$data['host'][] = 'proxy-003';
			$data['host'][] = 'proxy-004';
        }
		
		$data['owner'] = ($row['owner']?"{$row['owner']}":"{$GLOBALS['CONFIG']['LDAP_ADMIN']}");
		$data['member'] = ($row['owner']?"{$row['owner']}":"{$GLOBALS['CONFIG']['LDAP_ADMIN']}");
		
		return $data;
	}
	
	public function update($row)
	{
		$data = array();
		
		foreach( $this->schema as $value )
		{
			if( !is_array($value) )
			{
				if( $row[$value] )
					$data[$value] = $row[$value];
			}
		}
		
		return $data;
	}
	
}

?>

