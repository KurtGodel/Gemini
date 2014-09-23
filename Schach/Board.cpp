//
//  Board.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "Board.h"

const int RANDOM_ARRAY_LENGTH = 901;
uint64_t randomArr[RANDOM_ARRAY_LENGTH];

Board::Board()
{
    for(int i=0; i<64; i++)
    {
        b[i] = 0;
    }
    castling[0] = 0;
    castling[1] = 0;
    castling[2] = 0;
    castling[3] = 0;
    lastMove[0] = 0;
    lastMove[1] = 0;
    fiftyMoveDrawCounter = 0;
    recalculateBitboards();
    history.clear();
}

void Board::switchSides()
{
    if(history.size() == 0)
    {
        uint8_t i;
        for(i = 0; i < 64; i++)
        {
            if(b[i] == 0)
            {
                break;
            }
        }
        Move m;
        m.from = i;
        m.to = i;
        makeMove(m);
        fiftyMoveDrawCounter = 0;
    }
    
    uint8_t store;
    bool castleStore;
    
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 4; j++) {
            store = b[8 * i + j];
            b[8 * i + j] = b[8 * i + (7 - j)];
            b[8 * i + (7 - j)] = store;
        }
    }
    for(int i = 0; i < 64; i++) {
        b[i] *= -1;
    }
    
    
    int oldValue = lastMove[0];
    int promotion = oldValue/100;
    int x = (oldValue - 100*promotion)/8;
    int y = (oldValue - 100*promotion)%8;
    lastMove[0] = 8*x+7-y + 100*promotion;
    x = lastMove[1]/8;
    y = lastMove[1]%8;
    lastMove[1] = 8*x+7-y;
    
    
    lastMove[2] *= -1;
    castleStore = castling[0];
    castling[0] = castling[2];
    castling[2] = castleStore;
    castleStore = castling[1];
    castling[1] = castling[3];
    castling[3] = castleStore;
    for(int i = 0; i < history.size(); i += 5)
    {
        x = history[i]/8;
        y = history[i]%8;
        history[i] = 8*x+7-y;
        x = history[i+1]/8;
        y = history[i+1]%8;
        history[i + 1] = 8*x+7-y;
        history[i + 2] *= -1;
        store = history[i + 3]; // castling
        history[i + 3] = (history[i + 3] >> 2) & (history[i + 3] << 2);
    }
}

Board::Board(const Board& board)
{
    for(int i = 0; i < 64; i++)
    {
        b[i] = board.b[i];
    }
    for(int i = 0; i < board.history.size(); i++)
    {
        history.push_back(board.history[i]);
    }
    for(int i = 0; i < 13; i++) {
        bitmaps[i] = board.bitmaps[i];
    }
    lastMove[0] = board.lastMove[0];
    lastMove[1] = board.lastMove[1];
    fiftyMoveDrawCounter = board.fiftyMoveDrawCounter;
    for(int i = 0; i < 4; i++)
    {
        castling[i] = board.castling[i];
    }
    user = board.user;
    cpu = board.cpu;
    history.clear();
    for(int i = 0; i < board.history.size(); i++) {
        history.push_back(board.history[i]);
    }
}

void Board::setRandomArr()
{
    uint64_t randomArrayValues[901] = {0x6b8b4567327b23c6,
        0x643c986966334873,
        0x74b0dc5119495cff,
        0x2ae8944a625558ec,
        0x238e1f2946e87ccd,
        0x3d1b58ba507ed7ab,
        0x2eb141f241b71efb,
        0x79e2a9e37545e146,
        0x515f007c5bd062c2,
        0x122008544db127f8,
        0x216231b1f16e9e8,
        0x1190cde766ef438d,
        0x140e0f763352255a,
        0x109cf92eded7263,
        0x7fdcc2331befd79f,
        0x41a7c4c96b68079a,
        0x4e6afb6625e45d32,
        0x519b500d431bd7b7,
        0x3f2dba317c83e458,
        0x257130a362bbd95a,
        0x436c6125628c895d,
        0x333ab105721da317,
        0x2443a8582d1d5ae9,
        0x6763845e75a2a8d4,
        0x8edbdab79838cb2,
        0x4353d0cdb03e0c6,
        0x189a769b54e49eb4,
        0x71f324542ca88611,
        0x836c40e2901d82,
        0x3a95f8748138641,
        0x1e7ff5217c3dbd3d,
        0x737b8ddc6ceaf087,
        0x22221a704516dde9,
        0x3006c83e614fd4a1,
        0x419ac2415577f8e1,
        0x440badfc5072367,
        0x3804823e77465f01,
        0x7724c67e5c482a97,
        0x2463b9ea5e884adc,
        0x51ead36b2d517796,
        0x580bd78f153ea438,
        0x3855585c70a64e2a,
        0x6a2342ec2a487cb0,
        0x1d4ed43b725a06fb,
        0x2cd89a3257e4ccaf,
        0x7a6d8d3c4b588f54,
        0x542289ec6de91b18,
        0x38437fdb7644a45c,
        0x32fff902684a481a,
        0x579478fe749abb43,
        0x3dc240fb1ba026fa,
        0x79a1deaa75c6c33a,
        0x12e685fb70c6a529,
        0x520eedd1374a3fe6,
        0x4f4ef00523f9c13c,
        0x649bb77c275ac794,
        0x393865751cf10fd8,
        0x180115be235ba861,
        0x47398c89354fe9f9,
        0x15b5af5c741226bb,
        0xd34b6a810233c99,
        0x3f6ab60f61574095,
        0x7e0c57b177ae35eb,
        0x579be4f1310c50b3,
        0x5ff87e052f305def,
        0x25a70bf71dbabf00,
        0x4ad084e91f48eaa1,
        0x1381823a5db70ae5,
        0x100f8fca6590700b,
        0x15014acb5f5e7fd0,
        0x98a3148799d0247,
        0x6b9476442c296bd,
        0x168e121f1eba5d23,
        0x661e3f1e5dc79ea8,
        0x540a471c7bd3ee7b,
        0x51d9c564613efdc5,
        0xbf72b1411447b73,
        0x42963e5aa0382c5,
        0x8f2b15e1a32234b,
        0x3b0fd37968eb2f63,
        0x4962813b60b6df70,
        0x6a5ee6414330624,
        0x7fffca111a27709e,
        0x71ea1109100f59dc,
        0x7fb7e0aa6eb5bd4,
        0x6f6dd9ac94211f2,
        0x885e1b76272110,
        0x4c04a8af1716703b,
        0x14e17e333222e7cd,
        0x74de0ee368ebc550,
        0x2df6d64846b7d447,
        0x4a2ac31539ee015c,
        0x57fc4fbbcc1016f,
        0x43f1842260ef0119,
        0x26f324ba7f01579b,
        0x49da307d7055a5f5,
        0x5fb8370b50801ee1,
        0x488ac1a5fb8011c,
        0x6aa78f7f7672bd23,
        0x6fc75af86a5f7029,
        0x7d5e18f85f3534a4,
        0x73a1821b7de67713,
        0x555c55b53fa62aca,
        0x14fce74e6a3dd3e8,
        0x71c912989daf632,
        0x532999381fbfe8e0,
        0x5092ca791d545c4d,
        0x59adea3d288f1a34,
        0x2a155dbc1d9f6e5f,
        0x97e1b4e51088277,
        0x1ca0c5fa53584bcb,
        0x415e286c7c58fd05,
        0x23d86aac45e6d486,
        0x5c10fe21e7ffa2b,
        0x3c5991aa4bd8591a,
        0x78df6a5539b7aaa2,
        0x2b0d8dbe6c80ec70,
        0x379e21b569e373,
        0x2c27173b4c9b0904,
        0x6aa7b75c1df029d3,
        0x5675ff363dd15094,
        0x3db012b32708c9af,
        0x5b25ace2175dfcf0,
        0x4f97e3e453b0a9e,
        0x34fd6b4f5915ff32,
        0x56438d15519e3149,
        0x2c6e4afd17a1b582,
        0x4df72e4e5046b5a9,
        0x5d888a082a082c70,
        0x5ec6afd419e21bb2,
        0x75e0858a57a61a29,
        0x5399c65420ee1348,
        0x4427069ab37e80a,
        0x2157f6bc704e1dd5,
        0x57d2f10ebffae18,
        0xe3e47a82e48f044,
        0x49d0feac4bee5a5b,
        0x5551b9f324f6ab8e,
        0x634c574c24e99dd7,
        0x2a31b62d1849c29b,
        0x7dff9d09754342,
        0x69e7f3e52a6de806,
        0x1816f8c437df2233,
        0x7ab49daf759f82cd,
        0x61e74ea3597b4d84,
        0xf819e7f57c7d42d,
        0x312167ad631b64d4,
        0x78b5e77675486e47,
        0x6e534cde1a0dde32,
        0x65968c1c46263dec,
        0x260d8c4a73d4d3c4,
        0x746f2e306fde8af6,
        0x3fc32e2049c0e823,
        0x14d53685230f856c,
        0x6eaa85fb3f06ecb2,
        0x3b5948076caa2304,
        0x3f7c2ff425413bec,
        0x17180b0b579328b9,
        0x5d205e2011cca8ba,
        0x4d32ab863f07acc3,
        0x6b47f63e5cb44a05,
        0x16cf80f11c695dec,
        0x3fcfaed9f856867,
        0x11b1cc332e22fbb7,
        0x2993469977485850,
        0x744939a34fa0d2e3,
        0x6b1d2c1468b867d3,
        0x3f7f5dd92ae05a34,
        0x32794ff75454945e,
        0x4defdfa02123d5f2,
        0x135b811094927a8,
        0xdcdf8f652d7b105,
        0x2e8a639424e60401,
        0x2a6ad9bebaac1b4,
        0x36b2acbc779d8544,
        0x4ab26e7821faa2fa,
        0x5451cf496181ef69,
        0x3e6400e614217e23,
        0x710757d05015cd1a,
        0x424479da1a9a9e69,
        0x475e256a368db37e,
        0x6a3b714c327b517e,
        0x1f461b5129bacf25,
        0x5d5babb351bf6b48,
        0x7e0f63842b4b8b53,
        0x72e3413a116ae494,
        0x3494b2fbb13a31,
        0x64429599631f1690,
        0x25973e32ead6f57,
        0x6ec9d8445c49eaee,
        0x64af49b397c46bc,
        0x7e448de95a9cc3e5,
        0x1afe36253ca88ecf,
        0x6ebe4208c058df5,
        0xcbe5be93102bbe2,
        0x26a02c5e541c8153,
        0x67906f6010db9daa,
        0x697d2d26d68ab2,
        0x3a966cd063f37e85,
        0x5895f5fa38a5d054,
        0xf3f09d84b793735,
        0x4a10b4e843d3bcd4,
        0x4c2a71662e534a82,
        0x26f2d36471c1af98,
        0x3d00b9d915bcaba8,
        0x4e0b9a87434bae75,
        0x4f38f2654c502870,
        0x1de8725a6a37288a,
        0x8f8b73fca6b462,
        0x763cb68015b71329,
        0x3da970441cdce2de,
        0x69d3947c2539dfa5,
        0x2db88089706b674e,
        0x2c106a57684eed59,
        0x545ee5d34a66051,
        0x20f4bdad639defac,
        0x501f97866b057295,
        0x2771ac801c4a08ec,
        0x1958bd174e647fe4,
        0xe0bb885565976f1,
        0x64212b8c5c17530c,
        0x19a52566335a1df1,
        0x28677b7c378d97c0,
        0x1d91467c316032bb,
        0x44344c2213cdfcfc,
        0x471745e41ddbc66,
        0x30aadfda30eada61,
        0x27179c0b5e636063,
        0x215641af53280662,
        0x46b24dbc75b52783,
        0x57ce66b467a70b69,
        0x5953172f27edfe3a,
        0x52ac7dffc4c3af,
        0x443807276c053b16,
        0x4f2943935243bfac,
        0x425eb207334a6f1f,
        0x2e5b12b85c03d76d,
        0x66a48d1156c28e34,
        0x13916f2d435d38d,
        0x822c0ef57c5bb4f,
        0x1803d0894f3a06d4,
        0x59a377b648aeb063,
        0x24e135bb13c1,
        0x271210c7217b22e4,
        0x53e31a246dc45e83,
        0x17304a672bb180d8,
        0x556b69ed70836196,
        0x539f7f122817e7ec,
        0x7148254517d78639,
        0x141d2302407168d8,
        0x6a1b45e5567bd50a,
        0x73bbd7f81876589d,
        0x327fac775a606509,
        0x6f38e6d146111ba5,
        0x5e963896775ba7c1,
        0x1dd6d6f4769a091f,
        0x4695ae95777a4eaa,
        0x3f48b98246ba8fca,
        0x7835626c665aca49,
        0x6835b2ae4c187c90,
        0x541f28cd7f65fd16,
        0x77c9fd68298a92ba,
        0x6fe95eac4b697c7a,
        0x51a27aa6613183f2,
        0x634102b465bf9da8,
        0x21a2ecca4d5c4899,
        0x3c3b72b2155ec4c2,
        0x65d2a1376ebb1f2a,
        0x6fbf29cb550b8808,
        0x34cc3acf4e556261,
        0x4c672fc952a311c3,
        0x44ef6b8012fcde5e,
        0x4a1d606e4382503,
        0x59b76e284252c2da,
        0x6a92ef4c41ed20d7,
        0xe6b3f6a3eb21819,
        0x41531ded6353cd2,
        0x683caad3313c7c99,
        0x519eb94c39df2579,
        0x126e008b34dfbc00,
        0x1f9ec3223410ed56,
        0x23c049a5bda35d4,
        0x496fb218680ea5d1,
        0x4a9554fe392edbe4,
        0x3d1a2dd97f618fcd,
        0x7843e459815da3,
        0x5204a1914c73a9c6,
        0x1c7e3c011c2201ff,
        0x50abcec97635aa2a,
        0x5e74c4d93b3ebe15,
        0x3822cb016ce00443,
        0x79f0d62f7975e8ee,
        0x73154115622d8102,
        0x2ab2658744b3fa61,
        0x1c0ca67c3d206613,
        0x7993b6623bab699e,
        0x713153697bcfbafc,
        0x17859f723aa10581,
        0x63de60cd621af471,
        0x73cfe16520f88ea6,
        0x617c843e7b541fab,
        0x2a79ec49338125cf,
        0x47c7c97146f8284b,
        0x4fa327ce1873983a,
        0x3d2dd2752e17eca7,
        0x53b2564f75509d76,
        0x1af7f0ea4da32c7e,
        0x6ec68664e0d31ff,
        0x2fd0ad811978ebeb,
        0x52c12c614bdd53fd,
        0x569951fe4c54e2c3,
        0x788bd9b47caa567,
        0x48249dbf1f0e5d0d,
        0x26baae92c02fe8c,
        0x129517e763b8c4e,
        0x4cfb8d3262a5d5bd,
        0x718fabf97775797c,
        0x1626fb8c3957756a,
        0x3e6da1c765ca235b,
        0x51cb0da47b9b743c,
        0x13e21002257d63f4,
        0x70ec11b22eda00ed,
        0x732090725fb29816,
        0x3ce732ec22f13df3,
        0x792b8401fa85f4d,
        0x6ece91f04fc4d600,
        0x5bfd421076574f8b,
        0x178f7b672421dfcf,
        0x1565ac9919fb2650,
        0x5024de5b168efe17,
        0x1036b29f1d206b8e,
        0x7934d3d41c65e98,
        0x1495e50af5bcf61,
        0x3b1dd403530386d1,
        0x7525f2bcce8e1a7,
        0x4e9efb0d90802be,
        0x3266459b3f8b0cbf,
        0x37e203ab2586d60e,
        0x1f3da4d574c93698,
        0x48781401186928d6,
        0x47195e53746a5f2,
        0x682dfed6606ed7f6,
        0x2d9df57d7fbd7a3e,
        0x490b7c54303a216,
        0x19b8a08e54b59621,
        0x5992a02e29ef532d,
        0x71d601af52c77402,
        0x2bb5b1c666be6b9,
        0x6223436366d385c9,
        0x596f6d8a5749361f,
        0x73bc6770280e6897,
        0x605138de2622ad0c,
        0x6799755618333c89,
        0x4ba9831a6d71a2b,
        0xcfc73211421971b,
        0x1f404301116e0907,
        0x4b683d0d76e41d8,
        0x71dce0fd7906328b,
        0x72bbc16766d98c2,
        0x3c09d4a120e45ca4,
        0x4b232ee3159c74cf,
        0x4ad3afd23cf93092,
        0x6863e8d276896198,
        0x4365174b4a872c35,
        0x5d5ce7611cd484d5,
        0x21d0625551194ed1,
        0x44e2ed6c2219b33,
        0x773bfbdd2c7c62c2,
        0x1a54d7bc42e57ef7,
        0x33537ced27514ade,
        0x570716135293bfef,
        0x38bf53e5226f5320,
        0x5a0201c72a9c34e2,
        0x1b7585ab612dbddd,
        0x2109cda4577f5a4d,
        0x2121a816c2cfc88,
        0x6d1bcf1c4ce5ca53,
        0x29262d1a557fb7ee,
        0x436f2beb6c8b4466,
        0x2006e42420cc134c,
        0x95fc93b41d74679,
        0x71e5621e4e42b6a8,
        0x43f8e1ac69215dfb,
        0x7abf196a5e4db968,
        0x2c06dcf32e129658,
        0x59f044630df306,
        0xa656473e5e582b,
        0x257d46265aa8580e,
        0x68fa8d0d40f2cbd2,
        0x3bd615eba045ab2,
        0x1872261f3de8306c,
        0x7631573a58df53b,
        0xacdfac01f578454,
        0x5b0dad2a4e3d26ab,
        0xbe2c8ba7b14914e,
        0x6f0939f8154291f6,
        0x3cebd7c760ee9c16,
        0x6385489ee4b973,
        0x4a0ffa115e446208,
        0x5f3272db7616d704,
        0xc56f86064d17722,
        0x7924ca0acfd4ea7,
        0x232fcf4d1ea21031,
        0x67a5a6b5c2a5c5b,
        0x5f94dc03237bbca0,
        0x162eb70d78070222,
        0x6163ed0dc600e47,
        0x7d94f75d6c31e7cd,
        0x2bb7929b58a2a487,
        0x3a6f0e78379a5b56,
        0x53b735d529784870,
        0x4cdced4c10a30d9c,
        0xa66e486306235ea,
        0x1187c70f5476de98,
        0xea697f270ba39eb,
        0x4a8db59c1afd9053,
        0x558bb10d43b27fa7,
        0x27fadefa78bb805a,
        0x62548fd8fa085b0,
        0x4e5dcb541e96bdb,
        0x331c42501b1493c2,
        0x39f06dfd14802f5d,
        0x2774a2093785655a,
        0xb2172a532c34a5,
        0x102809e23b2125a3,
        0xac68ffb63df3fb7,
        0x64996e1357a37d47,
        0x74824d546f00529a,
        0x805b33160a1463,
        0x4377313216ac4b23,
        0x76c44e4ee04e6ce,
        0x31a9db764c4fff5b,
        0x51b7667559a4ba71,
        0x450b7fb6340bf64d,
        0x6945402149f15c6b,
        0x75f562281c618271,
        0x6505f02e2fe5d025,
        0x30e1b1cfc7a9237,
        0x676b35803193c8f9,
        0x5fa6c6dc77933f62,
        0x6cb4ee9c6a6d56d7,
        0x5b727f19514e5cb0,
        0x4210d41e4ff4cc6d,
        0x404eaf4a4a16874f,
        0x55fee0d13c5e07c,
        0x60c2d2734cc32f1f,
        0x11cac74a126cade9,
        0x19132e7b63822dc0,
        0x6c11685a5e1eae31,
        0x178e240d5556a87b,
        0x28100a9cd838636,
        0x71b82aedd15faca,
        0x3d69565b2299dcbc,
        0x19908d0224d48bdb,
        0x542da5b5793753de,
        0x1c67cb3d40e29452,
        0x63a4aab677da4a57,
        0x1230f10225b57ed4,
        0x47cf16c4527fa04c,
        0x6fcc06241dcdf795,
        0x564580c8508ed897,
        0x6a9126b568104812,
        0x62fb86803a45530,
        0x4b9275d24f0ceedb,
        0x61c30361632099e0,
        0x246397569d30dfd,
        0x70a42016161bc243,
        0x16e908c82e0d7671,
        0x38b59eff307995ca,
        0x52e2024dce344b5,
        0x29b0e9a86f49cd8a,
        0x4dc5d907d55945e,
        0x672417e15ff6ca09,
        0x330b13332ef32ea6,
        0x32766a5522d71957,
        0x4cc1263b8bbeb1d,
        0x7365f1ee37524cf0,
        0x70cc332f5661786e,
        0x3af6a2203c5ea902,
        0x256e67491cb9a581,
        0x1f7f42e249d1fea0,
        0x268cb37f102362f8,
        0x5fedc0e33d75bc47,
        0x3e30d96918a35fe3,
        0x6def52111112dbb6,
        0x2586a49817a03bb9,
        0x5ca941734c7d9f,
        0x24f5d0186780c122,
        0x534347a85800e34b,
        0x1673efc85b9b1fd,
        0x7ad7fca263351604,
        0xe759d1a6e3dee90,
        0x1a8762f47f41d049,
        0x449f66fe557e0515,
        0x3ba0794b6a0dce48,
        0x7237aa965b1fbc2d,
        0x33dfcce818c45e15,
        0x6b431f2513cd8dcb,
        0x563a1a5c2973f88f,
        0x2c70edae44296c6d,
        0x3a86d44551f79246,
        0x5bc9a8273ae37d86,
        0x45440fe5bf783f,
        0x22643ea91887578d,
        0x58c05b8a38d82e71,
        0x1e41098a5398582c,
        0x1c0d44752cb6a6a4,
        0x41d646bc3694a76a,
        0x2bf876ee675adba,
        0xc12ac7f6798f039,
        0x70837c027e4a5715,
        0x42b8ac67246348ea,
        0x170eb52b2dfbcb8c,
        0x3830d6b66d48cf87,
        0x576fc41b64a1c464,
        0x31723bf511f69861,
        0x369956abd3be41c,
        0x4cda15e77bdd6690,
        0xdfb5c5b6f3e5490,
        0x1464be1e66bbb7e5,
        0x2816830232a5c7a8,
        0x3a5410114423c777,
        0x5f5c6e4d7c2a56cd,
        0x7ab86ee1b54e53b,
        0x2a004876cb1b60,
        0x72edd5747323808a,
        0x515727635a681db,
        0x1786c9741c2427a1,
        0x63a24d684fb7a02a,
        0x96cf7283b121183,
        0x3459648f3adf331d,
        0x4d08a9e46af2bb3a,
        0x481b173919e2bfcc,
        0x66d021ca56167394,
        0x921145c7b34dfe8,
        0x3cd22b793137975e,
        0x2ddaa79177263b8a,
        0x755b5ed6d3715de,
        0x735092577013cdb7,
        0x188bfb1975f096df,
        0x76dee918b79d08d,
        0x691417697bf45b8e,
        0x412052699ae0dd,
        0x1818832f24c29fd1,
        0x5052810821857a57,
        0x5fd4b1544abe597,
        0x5c64ad752cdd5b39,
        0x6f9ea0d1247fc4ae,
        0x46c01b05566ec29b,
        0x7a9638434fe12f61,
        0x51a3a284376863bc,
        0x118c6c07f7e4a15,
        0x2e8e9f4776742596,
        0xcb55ff321df319e,
        0x6687f34d25415b0c,
        0x17cfc87d5d66dc65,
        0x30bb2b99e3dfe6,
        0x595b37f371db7e02,
        0x17ec0c47173bb22,
        0x169e1dd351d141cc,
        0x12f9357a7672cf28,
        0x567d27636f5de2ef,
        0x23502a61461bc834,
        0x13dda79d6a104566,
        0x1c8a8acfe73dfe0,
        0x39f174c76e2e2d53,
        0x45dc439d3b0a3b87,
        0x6dac7768746ae2e4,
        0x317e611d7a61d75b,
        0x164a14821806546b,
        0x1fa332672e19dd00,
        0x756d30d0505e5e01,
        0x2efdbce64ec868c4,
        0x4239dc03307c7daa,
        0x403c23e658d7f9d7,
        0x24dbf7653355960,
        0x4f4ac8ff58cae6d9,
        0x42933c4f729af360,
        0x1ee6af0d5670e3ed,
        0x5cab38c63b7139dd,
        0x64e4c3cd169cad8d,
        0x299f67302ac1076a,
        0x51a6e915174bde99,
        0x1f2bea4e3254a32,
        0x11adb5f43575fed1,
        0x1b2b9e9d3150e85c,
        0x638fdbd11098cf6e,
        0x1af465d128d98b7,
        0x5f61383243e92260,
        0x430a16621f9d5c18,
        0x1cc11c374557d5d8,
        0x72d2b5796c0be536,
        0x1e22bcb23565f1c8,
        0x5ea6d8963d096bbf,
        0xbd6d5b53b52115c,
        0x787aa59c70bb9983,
        0x51eebeea221a0ccd,
        0x1b7ca0ed2395a7ff,
        0x3965eb663aa88b3c,
        0x26baf2314b13a15a,
        0x701e8a0d41e690cf,
        0x7c6489b653ae65de,
        0x527f603d7e13d013,
        0x663bfe9531e0986f,
        0x41fcf274294614f7,
        0x517df4875ebe0eab,
        0x6e9dead04450aa00,
        0x4ac9f3e2cc0a782,
        0x79b69bc92970cc78,
        0x49ca134158d717e,
        0x64c2ddd54244b8de,
        0x76490b0136b19cbf,
        0x645ec5ab11c5abef,
        0x5a4744be1dc4b111,
        0x4c6e372b10236ef,
        0x68d8526b3c8cc138,
        0x42e8c7be653cdc22,
        0x103b2716156827fb,
        0x6350ac35767725ab,
        0x4748c06a254d9ea9,
        0x1fbd3aa318c6b4f2,
        0x40bad55e5b2573,
        0x5d175ef24ed5a137,
        0x1b1bccf556cdfabb,
        0x78466daf64e5e036,
        0x5c5b6c3a5d094b84,
        0x272a991452a4773b,
        0x13bae843b895ebf,
        0x646a232a6e022d01,
        0x294e0fd030d85a55,
        0x6f0463f11226623c,
        0x6d651b8d31ed2baf,
        0x77633e5e7da042a3,
        0x475553ab5ab3ea93,
        0x7417684fe9e1415,
        0x1893d13d4a2f2,
        0x2764c90740d3692,
        0x222fc86547c27fa,
        0x52e2d7c93d4b955a,
        0x5b4a22b54b294578,
        0x2231759037a58eef,
        0x283290fd495c0ea5,
        0xa4a062b3bed7940,
        0x54e56d646eb42955,
        0x29efa6427e337d35,
        0x1f8c83ab18f40a33,
        0x1059df71cf19f38,
        0x4ae135e27bd1dcf,
        0xa91e1dc1236898d,
        0x627108627ea94a2b,
        0x20d49da36272919f,
        0x127ded1d483966aa,
        0x667fc83134adb582,
        0x4cb58ea439629ffa,
        0x71f94adc27ffb15a,
        0x48be573142ac06c,
        0x5fa540492cbe7670,
        0x5d86cf1169ef4674,
        0x68abefb0326c3c76,
        0x58a36fca129b95f2,
        0x309fb9ab782ff375,
        0x2b8fa02540f9991c,
        0x52192ad7670d608,
        0x48b6b6ebfb37489,
        0x8a75f952b27bf4d,
        0xe5cbeb4297bfd38,
        0xd9a50ed20daabd1,
        0x71b563e3741a191e,
        0x558861533e6af287,
        0x2d7cb9194781ac2f,
        0x666aa3e132089e8c,
        0x5bac6c9c460fe42b,
        0x5ec714fc39333bad,
        0x2fff2a9f477304ac,
        0x6b9f78238a29a69,
        0x5a0e9a9f1c3f31ce,
        0xd28dde59e3ac4,
        0x5d38caea5f4208c,
        0x7c0f10cc25ef81d5,
        0x15a795154b67062,
        0x51174123240453ca,
        0x2e326d9a5eb19210,
        0x44deff9b1fe7d17d,
        0x52cbab2e1a6760ef,
        0x5e52c405486447,
        0x61e90d1e44bd67e6,
        0x325102d33d9579ba,
        0xacd4c11111817cf,
        0x76c8b5683acc76b1,
        0x588b1c7c62682d8b,
        0x436f111a3299b71b,
        0x7ea75f5a44419ef9,
        0x3837f1df5be02a44,
        0x4a35bf85344702ac,
        0x1cfac1a5fdd549a,
        0x38fd730e52e6ed3d,
        0x3e1a864672fe0a8,
        0x31987f4d48c0a800,
        0x717b2264642a7b,
        0x632808ef656a762b,
        0x4ac8ec34511160d,
        0x2a27de1136fd9196,
        0x2a68fc834f52a23,
        0x4815a966796f4530,
        0x6fc1a0d420a0c5e2,
        0x5bd772bb3330b1ee,
        0x533a7cfd5a7ed215,
        0x777250e7b726edc,
        0x365efc5a41a8106c,
        0x3fb97188382ea874,
        0x2185650778b6e496,
        0xb1595b125670d6b,
        0x5fe6c53f3cae14fe,
        0x6e27b56b66fe7765,
        0x41123f79514fbe5a,
        0x4c68ed9045bece3c,
        0x1660d4687690cba1,
        0x7cbc5fd319076430,
        0x2b85f5c444d20939,
        0x1276a9601b479698,
        0x6572cf1b6e4e1c1b,
        0x4e78488738ad4c18,
        0x48ccee3145ea996e,
        0x441fbaf47f2bea8b,
        0x792a9db3d92c7d,
        0x375a92ff29180ee2,
        0x7c901113427028b0,
        0x4e7f1c4d5c76d652,
        0x7f1e3dae3ca6d1b9,
        0x43754db740307d27,
        0xdf69013fde3b47,
        0x5ef4b642457647b,
        0x66f06e92abab37,
        0x3d5ec8ab31f4fcad,
        0x477db4704fd5720b,
        0x4d3c93462cf0838b,
        0x3e238e271bb4dbcd,
        0x659dcfa36f07c58,
        0x619f753b29bd8a97,
        0x61c66e369321f16,
        0x2d96b7143d76f9e2,
        0x124a2df82a26c828,
        0x7fe7229260c94a46,
        0x69d9e7a7f056040,
        0x1d701bff4a12ec32,
        0x3f35dd672b66ac12,
        0x59f12779452528cb,
        0x4fbe108e60602e62,
        0x47d0d402d1cd939,
        0x12552b10f4e8872,
        0x5cf24b455f91be56,
        0x3c3f0bfd1b15d96c,
        0x7b469a2321dcdba0,
        0x220655c45ce60f5e,
        0x4b9a66382822bca7,
        0x46182e7579311d4c,
        0x6599b68958625c6d,
        0x2357e5746580d91b,
        0x392ba6b329f583ef,
        0x6486395b569bc2b2,
        0x7408702123bc16c2,
        0x2026ec54df9979a,
        0x68e13f8e51c07f53,
        0x2e59c5fd30b21390,
        0x5edd588c40aef10d,
        0x40009c033bcfa3d1,
        0x2040af637c3fa800,
        0x56e57d3d1b874986,
        0x1e1c83a178ebd301,
        0x786d58e469b6e9d9,
        0x210e8fa83e858759,
        0x62e807256a84631,
        0x16e7e3c763fec9a,
        0x6c291f4c50138a7a,
        0x3035708950af58a7,
        0x26af4d2d243de0aa,
        0x746b6f6a28b1bbf2,
        0x723778445d4caef8,
        0x7a723b4520913e41,
        0xdfec288594f93d1,
        0x61402f4e4dff5e8b,
        0x151f37a3180deb1,
        0x4a3f068c6c04b4e0,
        0x1d082837685b8a2d,
        0x64f087e21575811c,
        0x521274065ff178a,
        0x53fb087534fa7b2b,
        0xca75dbc6ae2ec3c,
        0x3b3a67c578d07d08,
        0x3af676b76b6fd84e,
        0x497fd5b061a5c3e4,
        0xfadb8f83deb451a,
        0xa577fd61e5313d,
        0x1b37f4124c9bb1b,
        0x22766f7e2936b69a,
        0x5e194eec3b69ecd,
        0x773615267338868f,
        0x5377d7e41751bb2,
        0x5f3d3b70223fa5b6,
        0x29d0a5df442dc352,
        0x37b526d27be319e5,
        0x4a2cdadcbb02f47,
        0x30dd951056d43898,
        0x76931b846c17fcd6,
        0x4fa4b5a13189923b,
        0x5787d52419248b51,
        0x132f561f67358e1d,
        0x570fd06b1d86d5f5,
        0x691abf5a7247c47d,
        0x22509110b912ed8,
        0x1b7e7b1769dffc,
        0xf47cda512b4903d,
        0x73a2668c147f4b24,
        0x5429abef52dfa1fc,
        0x36bef0da7dfa51ce,
        0x170d654e6e7417ac,
        0x79dd6bb3613a402a,
        0x7a2446f32abb00c4,
        0x380e78c370b76277,
        0x16d2fd9a7b32e64,
        0x2240f4b26e5ad2be,
        0x20d7b9b535704ad1,
        0x559060db77e78a20,
        0x52f720c63eab2035,
        0x6a2f4e9d7547b1d6,
        0x4a3c4f0e5adc9b4,
        0x75b191d359841cb3,
        0x186259f26953f85f,
        0x6e0367d76c8c05e1,
        0x3c339a5b24c258b1,
        0x6a8657b05340ffa9,
        0x1336705d6463c363,
        0x347b3fd3d5ab751,
        0xf1ec4276c89b896,
        0x7e1219c825f1c1c1,
        0x743ce6fa20530e7b,
        0x144c94801514a0af,
        0x55c3594c69dcf55b,
        0xcfc2acf28ba7a13,
        0x28881591772b796c,
        0x1e022be972c4649f,
        0x7cd9432113b3bdbc,
        0x4c488152153b9d13,
        0x7d07b61b3a4be92a,
        0x1c7a2f4393b5076,
        0x5f0e41db6c4dfaa4,
        0xc7c501f7244b239,
        0x50b1be0840f78ff3,
        0x7f9f698a5fd0822f,
        0x2d8148897db18352,
        0x5c243f121be2f84,
        0x1e0491cd1a0ed871,
        0x36d2d03373c7eb1a,
        0x3ebcdcc43cefb03,
        0x1c82652d2c73e35d,
        0x3afa746f3a849116,
        0x1f3847fc37d3b790,
        0x4e384ed36b80c94f,
        0x4d0f54a34b4004ee,
        0x25ccb2794ed6f798,
        0x47b55654daf454,
        0x3b24f23c10f7a584,
        0x771fa68dbd6b044,
        0x51ef357776bf1017,
        0x6ba732747f707e01,
        0x7470936a71697665,
        0x212ead8512752537,
        0xb784ed658017db8,
        0x63d1051f641ca2,
        0x1bd078bb22bf757e,
        0x3bd8000056caed2b,
        0x5d4406955b1047fc,
        0xe9ea4bb2b7c5568,
        0x4691114b5badf95f,
        0x76bc5a566c5dc3c4,
        0x2a84f0f77b37afbb,
        0x7138b81965a9e333,
        0xc2f554068585ea6,
        0x718093785e1e8ab7,
        0x5f176ebe5d27c5ec,
        0x5d8f08b853880228,
        0x4e913c517ebdb63d,
        0x65fd275f5a098b27,
        0x56bf33f66c3a37b1,
        0x696da7c9728facb1,
        0xef9ad2f2545a7c9,
        0x495a99dc6c3db3c4,
        0x55efc657f93e98,
        0x17ba092c46e70111,
        0x33a737f7e766383,
        0x3344c4d65e2c28ee,
        0x9ae133e247d7cef,
        0x43d60c2115dd687e,
        0xcd5db9535569f99,
        0x73fbf3366bed4a53,
        0x127e6585518afbee,
        0x3f754c7b610fa1d6,
        0x5048b22c257273db,
        0x3b192cfd2707e622,
        0x11acab8c2486d4c7,
        0x199792d320a658bb};
    for(int i=0; i<RANDOM_ARRAY_LENGTH; i++)
    {
        randomArr[i] = randomArrayValues[i];
    }
}

bool Board::set(char* arr)
{
    bool rtn = true;
    for(int i=0; i<64; i++)
    {
        if(b[i] != arr[i])
        {
            rtn = false;
        }
        b[i] = arr[i];
    }
    for(int i=0; i<4; i++)
    {
        if(castling[i] != arr[64+i])
        {
            rtn = false;
        }
        castling[i] = arr[64+i];
    }
    if(lastMove[0] != arr[68])
    {
        rtn = false;
    }
    lastMove[0] = arr[68];
    if(lastMove[1] != arr[69])
    {
        rtn = false;
    }
    lastMove[1] = arr[69];
    if(fiftyMoveDrawCounter != arr[70])
    {
        rtn = false;
    }
    fiftyMoveDrawCounter = arr[70];
    
    history.clear();

    recalculateBitboards();
    return rtn;
}

bool Board::set(const Board &newBoard)
{
    bool rtn = true;
    for(int i=0; i<64; i++)
    {
        if(b[i] != newBoard.b[i])
        {
            rtn = false;
        }
        b[i] = newBoard.b[i];
    }
    for(int i=0; i<4; i++)
    {
        if(castling[i] != newBoard.castling[i])
        {
            rtn = false;
        }
        castling[i] = newBoard.castling[i];
    }
    if(lastMove[0] != newBoard.lastMove[0])
    {
        rtn = false;
    }
    lastMove[0] = newBoard.lastMove[0];
    if(lastMove[1] != newBoard.lastMove[1])
    {
        rtn = false;
    }
    lastMove[1] = newBoard.lastMove[1];
    if(fiftyMoveDrawCounter != newBoard.fiftyMoveDrawCounter)
    {
        rtn = false;
    }
    fiftyMoveDrawCounter = newBoard.fiftyMoveDrawCounter;
    
    history.clear();
    for(int i = 0; i < newBoard.history.size(); i++) {
        history.push_back(newBoard.history[i]);
    }
    
    recalculateBitboards();
    return rtn;
}

uint64_t Board::getZorbistHash()
{
    uint64_t index = 0;
    for(int i=0; i<64; i++)
    {
        index += randomArr[13*i+b[i]+6];
    }
    index += randomArr[832]*castling[0];
    index += randomArr[833]*castling[1];
    index += randomArr[834]*castling[2];
    index += randomArr[835]*castling[3];
    if(abs(b[lastMove[1]])==1 && abs(lastMove[1]-lastMove[0])==16)
    {
        // a pawn was moved two spaces last move
        if(lastMove[1]/8 == 0)
        {
            // left-most column
            if(b[lastMove[1]+8] == -1*b[lastMove[1]])
            {
                index += randomArr[836+lastMove[1]%100];
            }
        }
        else if(lastMove[1]/8 == 7)
        {
            // right-most column
            if(b[lastMove[1]-8] == -1*b[lastMove[1]])
            {
                index += randomArr[836+lastMove[1]%100];
            }
        }
        else
        {
            // middle column
            if(b[lastMove[1]+8] == -1*b[lastMove[1]])
            {
                index += randomArr[836+lastMove[1]%100];
            }
            else if(b[lastMove[1]-8] == -1*b[lastMove[1]])
            {
                index += randomArr[836+lastMove[1]%100];
            }
        }
    }
    
    // turn
    if(b[lastMove[1]%100] > 0)
    {
        index += randomArr[900];
    }
    return index;
}

std::string Board::toString()
{
    std::string rtn = "";
    for(int i=0; i<64; i++)
    {
        if(b[i] >= 0) {
            rtn += " ";
        }
        rtn += std::to_string(b[i]) + ", ";
        if(i%8 == 7)
        {
            rtn += "\n";
        }
    }
    rtn += std::to_string(castling[0]) + ",";
    rtn += std::to_string(castling[1]) + ",";
    rtn += std::to_string(castling[2]) + ",";
    rtn += std::to_string(castling[3]) + ",";
    rtn += std::to_string(lastMove[0]) + ",";
    rtn += std::to_string(lastMove[1]) + ",";
    rtn += std::to_string(fiftyMoveDrawCounter) + "\n";
    rtn += std::to_string(getZorbistHash()) + "\n";
    rtn += "history: ";
    for(int i = 0; i < history.size(); i += 5)
    {
        rtn += std::to_string((int)history[i]);
        rtn += " --> ";
        rtn += std::to_string((int)history[i + 1]);
        rtn += " [";
        rtn += std::to_string((int)history[i + 2]);
        rtn += "] [";
        rtn += std::to_string((int)history[i + 3]);
        rtn += "] [";
        rtn += std::to_string((int)history[i + 3]);
        rtn += "], ";
    }
    rtn += "\n";
    return rtn;
}

Board& Board::operator=(Board b)
{
    set(b);
    return *this;
}

bool Board::operator!=(const Board &board)
{
    return !(*this == board);
}

void Board::recalculateBitboards()
{
    for(int i = 0; i < 13; i++) {
        bitmaps[i] = 0;
    }
    for(int i = 0; i < 64; i++) {
        bitmaps[b[i] + 6] += (uint64_t)1 << i;
    }
    user = bitmaps[0] | bitmaps[1] | bitmaps[2] | bitmaps[3] | bitmaps[4] | bitmaps[5];
    cpu = bitmaps[7] | bitmaps[8] | bitmaps[9] | bitmaps[10] | bitmaps[11] | bitmaps[12];
}

void Board::updateTile(uint8_t tile, int8_t replace)
{
    uint64_t tile64 = (uint64_t)1 << tile;
    
    // make empty
    if(b[tile] != 0) {
        bitmaps[EMPTY] += tile64;
        bitmaps[b[tile] + 6] -= tile64;
        if(b[tile] < 0) {
            user -= tile64;
        }
        else {
            cpu -= tile64;
        }
    }
    
    // replace
    if(replace < 0) {
        bitmaps[EMPTY] -= tile64;
        bitmaps[replace + 6] += tile64;
        user += tile64;
    }
    else if(replace > 0) {
        bitmaps[EMPTY] -= tile64;
        bitmaps[replace + 6] += tile64;
        cpu += tile64;
    }
}

bool Board::operator==(const Board &board)
{
    for(int i = 0; i < 64; i++)
    {
        if(b[i] != board.b[i])
        {
            std::cout << "inequality due to tiles\n";
            return false;
        }
    }
    if(history.size() != board.history.size())
    {
        std::cout << "inequality due to history size\n";
        return false;
    }
    for(int i = 0; i < history.size(); i++)
    {
        if(history[i] != board.history[i])
        {
            std::cout << "inequality due to history element " << i << "\n";
            return false;
        }
    }
    for(int i = 0; i < 13; i++) {
        if(bitmaps[i] != board.bitmaps[i])
        {
            std::cout << "inequality due to bitmap[" << i << "]\n";
            return false;
        }
    }
    if(lastMove[0] != board.lastMove[0])
    {
        std::cout << "inequality due to lastMove[0]\n";
        return false;
    }
    if(lastMove[1] != board.lastMove[1])
    {
        std::cout << "inequality due to lastMove[1]\n";
        return false;
    }
    if(fiftyMoveDrawCounter != board.fiftyMoveDrawCounter)
    {
        std::cout << "inequality due to fiftyMoveDrawCounter\n";
        return false;
    }
    for(int i = 0; i < 4; i++)
    {
        if(castling[i] != board.castling[i])
        {
            std::cout << "inequality due to castling[" << i << "]\n";
            return false;
        }
        
    }
    if(user != board.user)
    {
        std::cout << "inequality due to user-bitmap\n";
        return  false;
    }
    if(cpu != board.cpu)
    {
        std::cout << "inequality due to cpu-bitmap\n";
        return false;
    }
    return true;
}

std::string Board::printBitmap(uint64_t input)
{
    std::string r = "";
    for(int i = 0; i < 64; i++) {
        if(i % 8 == 0) {
            r += "\n";
        }
        if(input % 2 == 1) {
            r += "1";
        }
        else {
            r += "0";
        }
        input >>= 1;
    }
    r += "\n";
    return r;
}

void Board::makeMove(const Move& move)
{
    history.push_back(move.from);
    history.push_back(move.to);
    history.push_back(b[move.to]);
    history.push_back((castling[0] << 3) + (castling[1] << 2) + (castling[2] << 1) + (castling[3] << 0));
    history.push_back(fiftyMoveDrawCounter);
    lastMove[0] = move.from;
    lastMove[1] = move.to;
    lastMove[2] = b[move.to];
    
    if(move.from > 100) {
        
        // promotion: from + 100 * value
        
        int8_t f = move.from % 100;
        int8_t piece = move.from / 100;
        if(b[f] < 0) {
            piece *= -1;
        }
        
        if(abs(lastMove[2]) == 4) {
            if(move.to == 0) {
                castling[3] = false;
            }
            else if(move.to == 56) {
                castling[2] = false;
            }
            else if(move.to == 7) {
                castling[0] = false;
            }
            else if(move.to == 63) {
                castling[1] = false;
            }
        }
        
        updateTile(move.to, piece);
        updateTile(f, 0);
        b[move.to] = piece;
        b[f] = 0;
        
        fiftyMoveDrawCounter = 0;
    }
    else if(abs(b[move.from]) == 1 && abs(move.from - move.to) > 2 && b[move.to] == 0) {
        // en passant
        if(b[move.from] == 1) {
            // cpu
            updateTile(move.to + 1, 0);
            updateTile(move.to, 1);
            updateTile(move.from, 0);
            b[move.to + 1] = 0;
            b[move.to] = 1;
            b[move.from] = 0;
        }
        else {
            // user
            updateTile(move.to - 1, 0);
            updateTile(move.to, -1);
            updateTile(move.from, 0);
            b[move.to - 1] = 0;
            b[move.to] = -1;
            b[move.from] = 0;
        }
        fiftyMoveDrawCounter = 0;
    }
    else if(abs(b[move.from]) == 6 && abs(move.from - move.to) > 10) {
        // castle
        if(b[move.from] > 0) {
            // cpu
            if(move.to == 23) {
                updateTile(7, 0);
                updateTile(23, 6);
                updateTile(31, 4);
                updateTile(39, 0);
                b[7] = 0;
                b[23] = 6;
                b[31] = 4;
                b[39] = 0;
            }
            else {
                updateTile(39, 0);
                updateTile(47, 4);
                updateTile(55, 6);
                updateTile(63, 0);
                b[39] = 0;
                b[47] = 4;
                b[55] = 6;
                b[63] = 0;
            }
            castling[0] = false;
            castling[1] = false;
        }
        else {
            // user
            if(move.to == 16) {
                updateTile(0, 0);
                updateTile(16, -6);
                updateTile(24, -4);
                updateTile(32, 0);
                b[0] = 0;
                b[16] = -6;
                b[24] = -4;
                b[32] = 0;
            }
            else {
                updateTile(32, 0);
                updateTile(40, -4);
                updateTile(48, -6);
                updateTile(56, 0);
                b[32] = 0;
                b[40] = -4;
                b[48] = -6;
                b[56] = 0;
            }
            castling[2] = false;
            castling[3] = false;
        }
        fiftyMoveDrawCounter++;
    }
    else {
        if(abs(b[move.from]) == 1 || b[move.to] != 0) {
            fiftyMoveDrawCounter = 0;
        }
        else {
            fiftyMoveDrawCounter++;
        }
        if(history[history.size() - 2] != 0)
        {
            if(b[move.to] == 4)
            {
                if(move.to == 63)
                {
                    castling[1] = false;
                }
                else if(move.to == 7)
                {
                    castling[0] = false;
                }
            }
            else if(b[move.to] == -4)
            {
                if(move.to == 56)
                {
                    castling[3] = false;
                }
                else if(move.to == 0)
                {
                    castling[2] = false;
                }
            }
            if(b[move.from] == 6) {
                castling[0] = false;
                castling[1] = false;
            }
            else if(b[move.from] == -6) {
                castling[2] = false;
                castling[3] = false;
            }
            else if(b[move.from] == 4) {
                if(move.from == 63)
                {
                    castling[1] = false;
                }
                else if(move.from == 7)
                {
                    castling[0] = false;
                }
            }
            else if(b[move.from] == -4) {
                if(move.from == 56)
                {
                    castling[3] = false;
                }
                else if(move.from == 0)
                {
                    castling[2] = false;
                }
            }
        }
        updateTile(move.to, b[move.from]);
        updateTile(move.from, 0);
        b[move.to] = b[move.from];
        b[move.from] = 0;
    }
}

void Board::unmakeMove()
{
    
//    std::cout << "unmakeMove(" << from << ", " << to << "); history.size() = " << history.size() << "\n";
    
    int32_t from = history[history.size() - 5];
    int32_t to = history[history.size() - 4];
    int32_t replaced = history[history.size() - 3];
    int32_t castle = history[history.size() - 2];
    fiftyMoveDrawCounter = history[history.size() - 1];
    history.erase(history.end() - 5, history.end());
    lastMove[0] = history[history.size() - 5];
    lastMove[1] = history[history.size() - 4];
    lastMove[2] = history[history.size() - 3];
    
    castling[0] = (castle & 8) > 0;
    castling[1] = (castle & 4) > 0;
    castling[2] = (castle & 2) > 0;
    castling[3] = (castle & 1) > 0;
    
    if(from > 100) {
        // std::cout << "un promote\n";
        int8_t f = from % 100;
        int8_t piece = from / 100;
        if(b[to] < 0) {
            piece *= -1;
        }
        
        uint64_t from64 = (uint64_t)1 << f;
        uint64_t to64 = (uint64_t)1 << to;
        
        // remove promoted piece from board
        bitmaps[piece + 6] -= to64;
        bitmaps[EMPTY] += to64;
        if(piece < 0) {
            user -= to64;
        }
        else if(piece > 0) {
            cpu -= to64;
        }
        
        // add replaced piece
        if(replaced != 0) {
            bitmaps[replaced + 6] += to64;
            if(replaced < 0) {
                user += to64;
            }
            else {
                cpu += to64;
            }
        }
        
        // add pawn
        if(b[to] < 0) {
            user += from64;
            bitmaps[USER_PAWN] += from64;
        }
        else {
            cpu += from64;
            bitmaps[CPU_PAWN] += from64;
        }
        
        b[f] = piece < 0 ? -1 : 1;
        b[to] = replaced;
    }
    else if(abs(b[to]) == 1 && abs(from - to) > 2 && replaced == 0) {
        // en passant
        if(b[to] == 1) {
            updateTile(to + 1, -1);
            updateTile(to, 0);
            updateTile(from, 1);
            b[to + 1] = -1;
            b[to] = 0;
            b[from] = 1;
        }
        else {
            // user
            updateTile(to - 1, 1);
            updateTile(to, 0);
            updateTile(from, -1);
            b[to - 1] = 1;
            b[to] = 0;
            b[from] = -1;
        }
    }
    else if(abs(b[to]) == 6 && abs(from - to) > 10) {
        // castle
        if(b[to] > 0) {
            // cpu
            if(to == 23) {
                updateTile(7, 4);
                updateTile(23, 0);
                updateTile(31, 0);
                updateTile(39, 6);
                b[7] = 4;
                b[23] = 0;
                b[31] = 0;
                b[39] = 6;
            }
            else {
                updateTile(39, 6);
                updateTile(47, 0);
                updateTile(55, 0);
                updateTile(63, 4);
                b[39] = 6;
                b[47] = 0;
                b[55] = 0;
                b[63] = 4;
            }
        }
        else {
            // user
            if(to == 16) {
                updateTile(0, -4);
                updateTile(16, 0);
                updateTile(24, 0);
                updateTile(32, -6);
                b[0] = -4;
                b[16] = 0;
                b[24] = 0;
                b[32] = -6;
            }
            else {
                updateTile(32, -6);
                updateTile(40, 0);
                updateTile(48, 0);
                updateTile(56, -4);
                b[32] = -6;
                b[40] = 0;
                b[48] = 0;
                b[56] = -4;
            }
        }
    }
    else {
        updateTile(from, b[to]);
        updateTile(to, replaced);
        b[from] = b[to];
        b[to] = replaced;
    }
}

uint8_t Board::lsm(const uint64_t& input) const
{
    return lsmArray[((input & -input) * 0x022fdd63cc95386d) >> 58];
}

uint8_t Board::decrementBitboard(uint64_t& input) const
{
    uint64_t r = input & -input;
    input -= r;
    return lsmArray[(r * 0x022fdd63cc95386d) >> 58];
}
