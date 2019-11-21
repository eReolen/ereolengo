<?php

/**
 * @file
 * Class representing a UNI•Login user.
 */

/**
 * Represents a UNI•Login user.
 */
class DingUniloginUser {
  protected $name;
  protected $authname;
  protected $authService;
  protected $userService;
  protected $institutionService;
  protected $profileData = array();
  protected $wsuser;
  protected $wspass;

  /**
   * Constructor.
   *
   * @param string $username
   *   The username of the user. Also default authname.
   * @param string $wsuser
   *   Username for the UNI•Login webservices.
   * @param string $wspass
   *   Password for the UNI•Login webservices.
   */
  public function __construct($username, $wsuser, $wspass) {
    $this->name = $username;
    // Authname defaults to same as username.
    $this->authname = $username;
    $this->wsuser = $wsuser;
    $this->wspass = $wspass;
  }

  /**
   * Get authorization service.
   *
   * @return DingUniloginWSIautor
   *   The authorization service.
   */
  protected function getAuthService() {
    if (!$this->authService) {
      $this->authService = new DingUniloginWSIautor($this->wsuser, $this->wspass);
    }

    return $this->authService;
  }

  /**
   * Get user service.
   *
   * @return DingUniloginWSIbruger
   *   The user service.
   */
  protected function getUserService() {
    if (!$this->userService) {
      $this->userService = new DingUniloginWSIbruger($this->wsuser, $this->wspass);
    }

    return $this->userService;
  }

  /**
   * Get instution service.
   *
   * @return DingUniloginWSIinst
   *   The instution service.
   */
  protected function getInstitutionService() {
    if (!$this->institutionService) {
      $this->institutionService = new DingUniloginWSIinst($this->wsuser, $this->wspass);
    }

    return $this->institutionService;
  }

  /**
   * Get user name.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Get authname.
   */
  public function getAuthName() {
    return $this->authname;
  }

  /**
   * Set authname.
   */
  public function setAuthName($name) {
    $this->authname = $name;
  }

  /**
   * Provide data for the users profile.
   *
   * @param string $field
   *   Name of the profile property.
   * @param mixed $value
   *   The value to set it to.
   */
  public function setProfileData($field, $value) {
    $this->profileData[$field] = $value;
  }

  /**
   * Get profile data.
   *
   * @return array
   *   Array of property => vaule pairs.
   */
  public function getProfileData() {
    return $this->profileData;
  }

  /**
   * The institution ids for this user.
   *
   * @var array
   */
  private $institutionsIds;

  /**
   * Get a whitelisted list of ids of user's institutions.
   *
   * @return array
   *   Institution ids.
   */
  public function getInstitutionsIds() {
    if (NULL === $this->institutionsIds) {
      $institutions = $this->getUserService()->getUserInstitutions($this->name);
      $this->institutionsIds = ding_unilogin_filter_institutions($institutions);
    }

    return $this->institutionsIds;
  }

  /**
   * The municipality ids for this user.
   *
   * @var array
   */
  private $municipalityIds;

  /**
   * List of institutions loaded in getInstitutionMunicipalities.
   *
   * @var array
   */
  private $institutions;

  /**
   * Get institutions.
   *
   * @return null|array
   *   The list of institutions.
   */
  public function getInstitutions() {
    return $this->institutions;
  }

  /**
   * Get the municipality IDs of the users institutions.
   *
   * @param null|array $institutionIds
   *   A list of institutions.
   *
   * @return array
   *   Municipality ids.
   */
  public function getInstitutionMunicipalities(array $institutionIds = NULL) {
    if (NULL === $this->municipalityIds) {
      $municipalityIds = [];

      if (NULL === $institutionIds) {
        $institutionIds = $this->getInstitutionsIds();
      }
      foreach ($institutionIds as $institutionId) {
        $institution = $this->getInstitutionService()
          ->getInstitution($institutionId);
        if (isset($institution->kommunenr)) {
          $municipalityIds[] = $institution->kommunenr;
          $this->institutions[$institutionId] = $institution;
        }
      }

      $this->municipalityIds = array_unique($municipalityIds);
    }

    return $this->municipalityIds;
  }

  /**
   * Check if user has been given a particular license.
   *
   * @param string $context
   *   Context to check.
   * @param string $project
   *   Project to check.
   */
  public function hasLicense($context, $project) {
    return $this->getAuthService()
      ->hasLicense($this->name, $context, $project);
  }

}
