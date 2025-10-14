#include <MYOS>

uint8_t status = 0;

DEFAULT_BOOT(0x100000, 0x500000, false);

using namespace myos;
//
using namespace myos::fdfs;

IDT idt;
Keyboard keyboard;
int keycount = 0;
char keybuffer[32] = { 0, };

// 문자열 비교
/*bool strEquals(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return false;
        a++;
        b++;
    }
    return *a == *b;
}*/

// 문자열 토큰화
//constexpr int MAX_TOKENS = 8;
//constexpr int MAX_TOKEN_LEN = 32;
/*int tokenize(const char* str, char tokens[MAX_TOKENS][MAX_TOKEN_LEN]) {
    int tokenCount = 0;
    int i = 0, j = 0;

    while (*str) {
        if (*str == ' ' || *str == '\t') {
            if (j > 0) {
                tokens[tokenCount][j] = '\0';
                tokenCount++;
                j = 0;
                if (tokenCount >= MAX_TOKENS) break;
            }
        }
        else {
            tokens[tokenCount][j++] = *str;
            if (j >= MAX_TOKEN_LEN - 1) j = MAX_TOKEN_LEN - 2;
        }
        str++;
    }
    if (j > 0 && tokenCount < MAX_TOKENS) {
        tokens[tokenCount][j] = '\0';
        tokenCount++;
    }
    return tokenCount;
}*/
//\

void panic() {
    console::clear();
    console::print("Frame DOS Panic!!!\n", 0, 0);
    while (true);
}

void version() {
    console::print("Frame DOS 1.0.4\nMONIKAS STOUDIAS FRAME DOS\n\
(C) Monikas Stoudias FuchsuaProject 2022, 2023, 2024, 2025");
}

bool shift = false;//''
bool waiten = false;

void input() {
    Key key = keyboard.readKey();
    if (!key.isKeyDown()) return;

    

    char c = key.chr();

    if (c == '\n' || c == '\r') {
        keybuffer[keycount] = '\0';
        console::print("\n");

        char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
        int argc = tokenize(keybuffer, tokens);

        if (argc > 0) {
            if (strEquals(tokens[0], "ver")) {
                version();
            }
            else if (strEquals(tokens[0], "shutdown")) {
                system::shutdown();
            }
            else if (strEquals(tokens[0], "reboot")) {
                system::reboot();
            }
            else if (strEquals(tokens[0], "mkdir") && argc > 1) {
                if (fdfs::mkdir(tokens[1])) console::print("Directory created\n");
                else console::print("mkdir failed\n");
            }
            else if (strEquals(tokens[0], "rmdir") && argc > 1) {
                if (fdfs::rmdir(tokens[1])) console::print("Directory removed\n");
                else console::print("rmdir failed\n");
            }
            else if (strEquals(tokens[0], "cat") && argc > 1) {
                if (!fdfs::cat(tokens[1])) console::print("cat failed\n");
            }
            else if (strEquals(tokens[0], "copy") && argc > 2) {
                if (fdfs::copy(tokens[1], tokens[2])) console::print("Copy OK\n");
                else console::print("Copy failed\n");
            }
            else if (strEquals(tokens[0], "move") && argc > 2) {
                if (fdfs::move(tokens[1], tokens[2])) console::print("Move OK\n");
                else console::print("Move failed\n");
            }
            else if (strEquals(tokens[0], "pwd")) {
                console::print("Current directory: ");
                console::print((char*)fdfs::getCurrentPath());
                console::print("\n");
            }
            else if (strEquals(tokens[0], "cd") && argc > 1) {
                fdfs::cd(tokens[1]);
            }
            else if (strEquals(tokens[0], "dir")) {// && arg c > 1
                fdfs::dir();
            }
            else if (strEquals(tokens[0], "cls")) {// && arg c > 1
                console::clear();
            }
            else if (strEquals(tokens[0], "info")) {
                if (strEquals(tokens[1], "cmd")) {
                    console::print("Monikas Stoudias Frame DOS 1.0\n\n\
Anus Sa\'operait Tamarils iat Frame DOS 1.x Quu maiken en \
Monikas Stoudias, Makamfin de Bourkouls\n\
\nver Seuen anus Tamarils\nshutdown Jayahen anus Tamarils\
\nreboot Rebaut anus Tamarils\nmkdir Maiken en Memoriyees\n\
rmdir Remowen en Memoriyees\n\
cat Seuen Fiils\ncopy Kaupiyeen Fiils\nmove Mowen Fiils\n\
pwd Seuen Rautet de Direktoriyees\ncd Tzhaingen Direktoriyees\n\
dir Direktoriyees de Azce\ncls Cloeren Skreuns\n\
info Pleiyeen ans Kommand\n\
                        ");
                }
                else if (strEquals(tokens[1], "release")) {
                    console::print("1.0.0\n")
                        ;
                    console::print("- Adden Kommands\n\
ver, shutdown, reboot, mkdirj, rmdir, cat, copy, move, pwd, cd\
\n, dir, cls, info\n\
\nMONIKAS_STOUDIAS/FUCHSUA_PROJECT/FRAME_DOS\
\n\
1.0.1\n\
- fix\n\
\n1.0.2\n\
- fix\n\
\n1.0.3\n\
- Kaupiyees Riipht\n\
\n1.0.4\n\
- Hardes Disk(kenselehe)\n\
- Shiftes Keyees\n \
- Kaupiyees Riipht\n\
");
                }
            }
            else if (strEquals(tokens[0], "infoDisk")) {
                console::printInt(status, 15, 13);
            }
            else {
                console::print("Unknown command: ");
                console::print(keybuffer);
                console::print("\n");
                char counterchar[25];
                intToStr(keycount, counterchar);
                console::print(counterchar);
                console::print("\n");
            }
        }

        keycount = 0;
        keybuffer[0] = 0;
        return;
    }

    if (key.scancode() == Scancode::LEFT_SHIFT || key.scancode() == Scancode::RIGHT_SHIFT) {
        if (key.isKeyDown()) shift = true;
        //else if (key.isKeyUp()) shift = false;//u
        else shift = false;
        //return;//
    }
    //else ////

    if (keycount < 31) {//) && key.isKeyDown()
        if (c == '\b') {
            if(keycount != 0){
                keycount--;
                //ujiasdhuif vuyhidseahfuiohweruiorhuiewhjfuji nvm
                console::print(c);
            }
        }
        else if (waiten) { //c == 0x2a || c == 0x36
            keybuffer[keycount++] = shiftChar(c);
            console::print(shiftChar(c));
            waiten = false;//
            shift = false;
        }
        else {
            if(!shift){//s
                keybuffer[keycount++] = c;
                console::print(c);
            }
            else {
                waiten = true;//
            }
            //
        }
    }
}

bool anm = true;

void pausee() {
    Key key = keyboard.readKey();
    if (!key.isKeyDown()) return;



    char c = key.chr();

    if (key.scancode() == Scancode::LEFT_SHIFT || key.scancode() == Scancode::RIGHT_SHIFT) {
        if (key.isKeyDown()) shift = true;
        //else if (key.isKeyUp()) shift = false;//u
        else shift = false;
        //return;//
    }
    //else ////

    if (keycount < 31) {//) && key.isKeyDown()
        if (c == '\b') {
            if (keycount != 0) {
                keycount--;
                //ujiasdhuif vuyhidseahfuiohweruiorhuiewhjfuji nvm
                console::print(c);
                anm = false;
            }
        }
        else if (waiten) { //c == 0x2a || c == 0x36
            keybuffer[keycount++] = shiftChar(c);
            console::print(shiftChar(c));
            waiten = false;//
            shift = false;
            anm = false;
        }
        else {
            if (!shift) {//s
                keybuffer[keycount++] = c;
                console::print(c);
                anm = false;
            }
            else {
                waiten = true;//
            }
            //
        }
    }
    //keyboard.disableInterrupt();
}



void Main(uint32 magic_number, GrubBootInfo& boot_info) {
    PIC::initialize();
    idt.initialize();
    idt.setHandler(0, panic);
    idt.setHandler(6, panic);
    idt.setHandler(7, panic);
    idt.setHandler(8, panic);
    idt.setHandler(33, input); // 키보드 인터럽트
    idt.load();
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x00);
    outb(0x3D4, 0x0B);
    outb(0x3D5, 0x0F);

    status = inb(0x1F7);
    //char fff[26];
    //intToStr((uint64)status, fff);
    console::printInt((uint64)status, 15, 4);//Hexfff

    //anm = true;
    //while (anm) pausee;

    uint16_t posss = 0;
    outb(0x3D4, 0x0F);
    outb(0x3D5, posss & 0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (posss >> 8) & 0xFF);

    //fdfs::init();//init();/writeBlock

    console::clear();
    //console::print("Frame DOS 1.0.0\n\n");
    //console::print("");//""
    //fdfs::init();//dddajsdlkfjlksdjflkjsdlkaaaaa
    version();

    keyboard.enableInterrupt();

    while (true) {
        asm volatile("hlt");
    }
}
