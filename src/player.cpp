class Player {
  public:
    Player();
    Player(Player&&) = default;
    Player(const Player&) = default;
    Player& operator=(Player&&) = default;
    Player& operator=(const Player&) = default;
    ~Player();

  private:
    int socket;
    bool connected;
};

Player::Player() {
}

Player::~Player() {
}
