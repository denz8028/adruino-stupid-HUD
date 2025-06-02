class OutSerial
{
public:
    void SendPlayerDataToArduino();
    // void Update();
private:
    int m_iHealth;
    char m_pszWeaponClassname[100];
    float m_flFlashlignt;
    bool m_bGameState;

};
