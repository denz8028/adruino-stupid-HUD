//=================== den4iklovelinux (none copytights) =============
//
// Keywords: Serial
//
//===================================================================
//
// TODO/FIXME: Implement standalone Think (or update for non source-modders) method
// TODO/FIXME: Implement getting chapter name

#include "dbg.h"
#include "strtools.h"
#include "tier0/platform.h"
#include "convar.h"
#include "vgui/ILocalize.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "cbase.h"
#include "c_basehlplayer.h"
#include "out_serial.h"
#include "cdll_util.h"
#include "iclientmode.h"


//LAST INCLUDE ALWAYS!!!

#include "tier0/memdbgon.h"
// Setup serial dev (Arduino)
static int serial_fd = -1;
static const char* SERIAL_PORT = "/dev/ttyUSB0"; // Arduino device !!! Change if needed
bool InitSerialPort() {
    serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    if (serial_fd < 0) {
        Warning("Unable to open %s\n", SERIAL_PORT);
        return false;
    }

    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0) {
        Warning("Error on getting parameters\n");
        close(serial_fd);
        serial_fd = -1;
        return false;
    }

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | ISTRIP);

    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    tty.c_cc[VTIME] = 5;
    tty.c_cc[VMIN] = 0;

    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        Warning("Error on configuring port\n");
        close(serial_fd);
        serial_fd = -1;
        return false;
    }

    Msg("Serial device %s succeeded\n", SERIAL_PORT);
    return true;
}

// Send data to Arduino (in "str1|str2|str3|str4\n" format)
void SendToArduino(const char* str1, const char* str2, const char* str3, const char* str4) {
    if (serial_fd < 0 && !InitSerialPort()) {
        return;
    }

    char buffer[256];
    Q_snprintf(buffer, sizeof(buffer), "%s|%s|%s|%s\n", str1, str2, str3, str4);

    if (write(serial_fd, buffer, strlen(buffer)) < 0) {
        Warning("Ошибка отправки данных\n");
        close(serial_fd);
        serial_fd = -1;
    }
}

// Send some data to test connection
CON_COMMAND(send_game_data, "Send data to Arduino") {
    if (args.ArgC() < 4) {
        Warning("Ussage: send_game_data <str1> <str2> <str3> <str4>\n");
        return;
    }

    SendToArduino(args.Arg(1), args.Arg(2), args.Arg(3), args.Arg(4));
}

// void OutSerial::Update()
// {
//     C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
//     C_BaseCombatWeapon *wpn = GetActiveWeapon();
//     int realHealth = pPlayer->GetHealth();
//     float realFlashlight = pPlayer->m_HL2Local.m_flFlashBattery;
//     // const char* realWeapon = wpn->GetClassname();
//     if (!wpn || !pPlayer )
//         return;
//     if (realHealth != m_iHealth || realFlashlight != m_flFlashlignt)
//     {
//         // m_iHealth = realHealth;
//         // m_flFlashlignt = realFlashlight;
//         // V_strcpy(m_pszWeaponClassname, realWeapon);
//         SendPlayerDataToArduino();
//     }
// }
// Send some player data to our "HUD" (adapted to lostinshit)
void OutSerial::SendPlayerDataToArduino() {
    C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
    if (!pPlayer) return;
    IClientMode *icm;
    C_BaseCombatWeapon *wpn = GetActiveWeapon();
    char wpnResult[100];
    Q_snprintf(wpnResult, sizeof(wpnResult), wpn->GetClassname() + 7);
    // "Mod name|GameState|Current weapon |Health and flashlight (lish moment)"
    SendToArduino(
        "Modname",
        "InGame",
        wpnResult,// angStr,
        VarArgs("HP: %d   Flash: %.0f", pPlayer->GetHealth(), pPlayer->m_HL2Local.m_flFlashBattery)
    );
}
