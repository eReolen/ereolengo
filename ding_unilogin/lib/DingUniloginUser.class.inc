<?php

/**
 * @file
 * Class representing a UNI•Login user.
 */

/**
 * Represents a UNI•Login user.
 */
class DingUniloginUser {
  /**
   * TODO missing member variable doc comment.
   *
   * @var string
   */
  protected $name;
  /**
   * TODO missing member variable doc comment.
   *
   * @var string
   */
  protected $authname;
  /**
   * TODO missing member variable doc comment.
   *
   * @var string
   */
  protected $authService;
  /**
   * TODO missing member variable doc comment.
   *
   * @var string
   */
  protected $userService;
  /**
   * TODO missing member variable doc comment.
   *
   * @var string
   */
  protected $institutionService;
  /**
   * TODO missing member variable doc comment.
   *
   * @var string
   */
  protected $profileData = array();
  /**
   * TODO missing member variable doc comment.
   *
   * @var string
   */
  protected $wsuser;
  /**
   * TODO missing member variable doc comment.
   *
   * @var string
   */
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
      // Get additional fake municipality data.
      $fake_municipalities = _ding_unilogin_get_nonstitution_municipalities();
      foreach ($institutionIds as $institutionId) {
        $institution = $this->getInstitutionService()->getInstitution($institutionId);
        // Add or overwrite kommunenr from fake municipality if it's defined.
        if (isset($institution->instnr, $fake_municipalities[$institution->instnr]['kommunenr'])) {
          $institution->kommunenr = $fake_municipalities[$institution->instnr]['kommunenr'];
        }
        if (isset($institution->kommunenr)) {
          $municipalityIds[] = $institution->kommunenr;
          $this->institutions[$institutionId] = $institution;
        }
      }

      $this->municipalityIds = array_unique($municipalityIds);
    }

    return $this->municipalityIds;
  }

}
