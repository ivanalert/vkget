#include <QtTest>

#include "vkontakte.h"
#include "vkresponse.h"
#include "playlist.h"
#include "vkitemmodel.h"
#include "downloadmanager.h"

class TestVK: public QObject
{
    Q_OBJECT

    void fillIdData();

    VKontakte *m_vk;

private slots:
    void initTestCase();
    void loadUser_data();
    void loadUser();
    void loadFriend_data();
    void loadFriend();
    void loadGroup_data();
    void loadGroup();
    void decodeUrl_data();
    void decodeUrl();
    void testPlaylist();
    void cleanupTestCase();
};

void TestVK::fillIdData()
{
    QTest::addColumn<QString>("user");
    QTest::newRow("id: 1300635") << "1300635";
    QTest::newRow("id: 3352952") << "3352952";
    QTest::newRow("id: 3653482") << "3653482";
    QTest::newRow("id: 4130062") << "4130062";
    QTest::newRow("id: 4554180") << "4554180";
    QTest::newRow("id: 4661045") << "4661045";
    QTest::newRow("id: 5639393") << "5639393";
    QTest::newRow("id: 10621599") << "10621599";
    QTest::newRow("id: 11948934") << "11948934";
    QTest::newRow("id: 14386259") << "14386259";
    QTest::newRow("id: 18630849") << "18630849";
    QTest::newRow("id: 26832978") << "26832978";
    QTest::newRow("id: 32342339") << "32342339";
    QTest::newRow("id: 32621431") << "32621431";
    QTest::newRow("id: 33304745") << "33304745";
    QTest::newRow("id: 35830310") << "35830310";
    QTest::newRow("id: 61530374") << "61530374";
    QTest::newRow("id: 63723790") << "63723790";
    QTest::newRow("id: 66200168") << "66200168";
    QTest::newRow("id: 71161611") << "71161611";
    QTest::newRow("id: 72720669") << "72720669";
    QTest::newRow("id: 79035768") << "79035768";
    QTest::newRow("id: 91552433") << "91552433";
    QTest::newRow("id: 103323234") << "103323234";
    QTest::newRow("id: 104971323") << "104971323";
    QTest::newRow("id: 136239384") << "136239384";
    QTest::newRow("id: 165382854") << "165382854";
    QTest::newRow("id: 171304459") << "171304459";
    QTest::newRow("id: 186094548") << "186094548";
    QTest::newRow("id: 214785589") << "214785589";
    QTest::newRow("id: 278277320") << "278277320";
    QTest::newRow("id: 335770395") << "335770395";
    QTest::newRow("id: 346321028") << "346321028";
    QTest::newRow("id: 381498973") << "381498973";
    QTest::newRow("id: 486366833") << "486366833";
}

void TestVK::initTestCase()
{
    //Token is a private info! Load from file.
    m_vk = new VKontakte(QStringLiteral("5.100"), QStringLiteral("TOKEN"), this);
}

void TestVK::loadUser_data()
{
    fillIdData();
}

void TestVK::loadUser()
{
    QFETCH(QString, user);

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("user_ids"), user);
    auto res = m_vk->apiRequest(QStringLiteral("users.get"), query);
    QTest::qWaitFor([res] { return res->isFinished(); });

    bool ok = false;
    const auto obj = VKResponse::responseOrError(res->readAll(), ok);
    QVERIFY(ok);

    //3 requests in 1 sec maximum.
    QTest::qSleep(334);
}

void TestVK::loadFriend_data()
{
    fillIdData();
}

void TestVK::loadFriend()
{
    QFETCH(QString, user);

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("user_id"), user);

    auto res = m_vk->apiRequest(QStringLiteral("friends.get"), query);
    QTest::qWaitFor([res] { return res->isFinished(); });

    bool ok = false;
    const auto obj = VKResponse::responseOrError(res->readAll(), ok);
    QVERIFY(ok);

    //3 requests in 1 sec maximum.
    QTest::qSleep(334);
}

void TestVK::loadGroup_data()
{
    fillIdData();
}

void TestVK::loadGroup()
{
    QFETCH(QString, user);

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("user_id"), user);

    auto res = m_vk->apiRequest(QStringLiteral("groups.get"), query);
    QTest::qWaitFor([res] { return res->isFinished(); });

    bool ok = false;
    const auto obj = VKResponse::responseOrError(res->readAll(), ok);
    QVERIFY(ok);

    //No more 3 requests in 1 sec.
    QTest::qSleep(334);
}

void TestVK::decodeUrl_data()
{
    QTest::addColumn<QByteArray>("encoded");
    QTest::newRow("Vangelis  - Chariots Of Fire (Hoyaa Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdviC1DontiOuJLhDwTYm1npC2r5lLDumwLKDMjKws9nyNjnrNmZlNHgtgKWCNjfqJrHsxv5mtDTEfLLAdnHzdfLDgv1C1znqN02Ddjwrhr3ChziC25MytnOmtHestqVytCOzgrdtxb1rgvPmxLKnZ9SnKy2ms5LBZKWzJfhvxH2sMCTBgn5qJDTme1jthjuA2qOuLnbqZDeyvjmoMfMC3bWBhf2nhPJrZj0yY1jDI1rsLf6zdfMAgfUvtzdquXZyw8YAJLhANyZzwDvuhnpmc9ol1jny1q9lY9fn29QodzRAeDF#AqSXnZa");
    QTest::newRow("Mohombi - Hello") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=zhblBKyOChbQmuXiALPLr25ZywjVrtfOzgH4ztOXrKfXsOnqCKnZDJnHBtnICNndmOuYruDWnujjzZD1uNvslZm4ngOOzLq3As9HuM41vJHZr3fIuO1VoKuOndbgEsO3y28YyKCZlJflwxyUvxbqmvPtr3b1rNHszdbnz3vOyKSUnNPSoh0TweHXxOn6ofnPELvdmZjUmNzWt3vUwLfFutf1xZiXnJe3tdq2mxroqNzcsgj1ohi5ugHLnwrKos1HpOnNz24VywTIzu56Bxn0l1rOyq#AqS1mtK");
    QTest::newRow("Tom Swoon &amp; Kerano feat. Cimo Fr&#228;nkel  - Here I Stand  ") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfuOC3C2BdfzzLDHqNjKvJjnDNPkmJPTCOqVEhrzv2vZBdu3C1i4oeDssgvtngrkyZrtBxiZDhbRotHRndLIAM1JsMvHuJ9YlMvfrNyZBtDVyMD2zOqXmhbwDtnin28WndDeyOvkreiYqZfpC3q2ys8Tm1jysu8YAtfUnKCVl30VAhq1mgLFBxvjtw94uNrguuTRDOLVC2TezNaWuwzMvurelJLQDNv4mdDZmNjLnuDIDMOXoxHlodbJpxvZzNDYrJrNCY4ZqJjMCgfWnO9dte5UuOi3y2uZzfzRmLCVu1vkyvfW#AqS3mde");
    QTest::newRow("Бьянка - Музыка (Club Stars Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhLTEdfMAxLLBLnqCdfIyxbhAZ0YELzjCgriBxzHwMCZmYOZnNrKrxvqzxH1n2uVy3aWnLq2Dc5Hohn5BOrtyIO3wxfRrY9ZuO04BdbPztuOC1PUDwqOx21OouH3rwX3ow1vmOS4yJzIAM9IBK94CwvrCtyUs1qUnhvJx3vUqN01Eujmn3fZC2n2ptzpnZjSudruqO0YzvjYyOHSl10VzxfXrZnVnI9lqv9zsvi2DfHZpOLcExjivM9qngfWmZzFmhvum2nMndK5ALHHnMjlDNbwBL01zJi5l1PWshqWzxrlCG#AqS1mdq");
    QTest::newRow("Blondie - Maria") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=zdLYpwjOA2X0DMjJmfjXCOvouvDZuOX0mMu3n2y4sNnoAJL1DhvtzKf6qY9Orw5YneqXpY9xx3vbBKfPDMu6rK1Om1u1n1G4xOv4swuUx2vlxOiTmY1ICY1KvY5SvOzVluSWzw9TlNLdt1LkCe9KCOLZzxbdyuW1A29vv2funJjJD2uWy1KZC2mZExPrvJHIBMPyAY9OB2LeBhvquKDWBxzHttzuyZGWBOf4sfHcsZbfCxuZx21dns81vwrOzgrOmtnIohuXBKLJrxbxDdi1zgSYoa#AqSYodu");
    QTest::newRow("Fort Minor - Where&#39;d You Go (Nick Veech Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=qwrYsu4YEgnfAhC4Cge4DueUodv6rM5jlZK1zwvJB2LvmgnuCMHrmg11sO4VAu5ulZy6BOrQteXOnLflzgK9qtrUrgK1rezsqKfzCMvRBdHOnfLYsvmYBtndzgzUExrWzgvRuNuTDxe4ELPctMzjBwf4rMfqwJm4zeLJCLDZAMjeyxvWC3nuDhnPtfjNogHAnxnWA3nHB3PLownsohm1ywHHwgnmyZfPEL84C3CVExi3CZL2yZz1mwqWrfK2l1GVlMOZz2yODNu4CtrsqZfbohm4BKX5rvq/lLe4yMWYCOnZqKO#AqS1ntu");
    QTest::newRow("Deepside Deejays - Never Be Alone") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AefqsI5Qx2rLoe9VCde5mdf5zhLMy1rUqO8Tmg9IDxf1rc1Mzv95zMfRyufbwvnNzZzRBdC3ngr2Dc9pC1uYAxzzC3CVzhr4mgrquZL0Edz2te9OyK9XmY5br3n6B250qKO3BZ1KnMH1mg9zmf9JD3rLzxbVv3HUnLLWAgvOnOnJqs5ytg03Bs1gCdnlA3bcngnptf93oez2oxe4zu1Qnf9Lq3G5me9UnuHuBffHlZnMueTRBNjfutHLDLeWBZP1p2jpAhj5wgfnwgrTCNzcl2Hhta#AqSOmJi");
    QTest::newRow("P!nk - U + Ur Hand") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=wur1DNn4EtrRDLDxl2vVnfjUvM1WouDdB2S3z2DWvMPWCL0UC1H0rguVnxaWDe41COHcvgy5BK12qLzVrKWXvuCUofzfsum1C1G4tOGUEuzTqxbdAunTnNrrrI8VBdvyuMP1yZuOAxjhmNbcmwDVztf2rwuYDdi3yZj6B1e2q19OmxLVytm9y1D4C3qXmvfNotu6r3vtwen3EdfpmM0XnM5UCgvOnujQB1uXzc11p25iAezMt3vHtxzmAJbRmurdAgm4rerevwDMBOiWBdfimwGTvMm#AqSOnJu");
    QTest::newRow("The Immortals - Rayden (Eternal Life)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=BZvnBZaYzuDQyJnhyZHKmw9imgnHDJmXwgTSDNmTCfLOqJzAtwzLm3m4ndPLBZy4wxvvve5LoezHte4YxY9yCY8Um3rQovrRmgzbmhaUufjYzKDOAwH5nNrfthiVseKOov9emgfNyxrsAgr0ngDxztf2mNPhtJG2BxCXsuf2uLv3lM4Wyxa9CfzvuhfKuvnWsxH4nhuWwv9XwKuZEM9bl3LJoxDZnNfdtwzgDLHLpZHbAhfKCLrWv2nPzdjRmwPhoeSZthqTEuG3vMvIrhPZv1CZCuO#AqSXndK");
    QTest::newRow("Scissor Sisters - I dont feel like dancing (Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=zdvHB3DrndjQAZvxx2jHEMHZnvjzl1u5EwPKAZbPl3G4mhr4qJDwm3bJpxrwAffVBcO2Aezey1fFzgfZBtzWqMP5zevUlw9ynKzJmxG1yunTCY42CwDiuxr5vxmWm3zWs1e6lOuZmxrgwNG2tfnSrxb0C2vHwJvWztvKnNrrDg5XDdvnrwjNs2PsmtDrqJ9YyuTnzxnslte2vdvrnhvkzxKYugq2zuC5lI43Aejemwv1rZfmDhi3u1ritMvllw1YshvJv1LPEgm1txzbnwrHlOXvrxy#AqS1ntm");
    QTest::newRow("Kapral &amp; Osya - Ты Не Верь Слезам (Cover)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AenwztuYqJn1rtrFzw9clwPcvwnios9OtKvIyxi5BJLlvdfuDhH2zfLuog8UDdu3n3eYC2SZAxyYywHJAZ9ZovDLzLPYlOrhzhnnlMr4CgvdtZLAy3zzl3fImdnHCtLeBgzrq1fsDfGYu1H1Dxj3s3bToKX1sNHhqNG1DfzSyZK3yvPxmu8VnxmWmwPUmKvVls49ntvfvfLcEhPfswjKrwWYDuH2rZjLDe1jztqXDhbXv2CZzgrnCg1Zsve4qJu2t1u4qtjkDgXlzviXnOX1rZKXtW#AqSZodi");
    QTest::newRow(" Yiruma &amp; Skullee - River Flows In You Dub Step MIX") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=mJeXBZf3t2X3x3vWwJnnrJrTEKrJufKVDwjWCgrPAOLQm2qZyODPtd1nz3zxCdvQqJzYC2HKDfHOC3aUDdqWmJnUn2Djt2iOtfvuvhPHsgj2ruv4sY5Kngj3ugnyA1qVvOTKt3G4ms84B2rczhjUDtzHBdfsuKjRsMjMzxn3CJHXyZHwDdnYEtjHEI81m1fLrvzYD3qZBhzUDvntwgeOztrFChfLvwjOtJvWDfLLotHOnwDIwdi6C3HMx1i2BMz3EtfdpYOUrs1RowPNmOP4yxrF#AqS3mdK");
    QTest::newRow("Pitbull feat. TJR - Don't Stop the Party") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=nOf4nNngnc8ZCOvvDhzTre4OowL1ExyWnMW/mhvOuMrFqtr4ztGVzxzIyI14n2rKtxPKr3HtngTZmvC3DI42uZzRn1Dimw4UyMzFDdaOEhjInMjxvw0VlMHgDtrOnOTlrtnoBxbPnwC4sI9eB2f4odzJCey5AOfvmfvLBgHMCeviCNfYuwuTwePHvKq4meLTtxbjvNnJugTiDgHyC2D0mwfWzezRDNzlzKXImu1opxfAoMmYy3jpDgzLDgu4BOGWAvu3mZj5sMfUAfDLxOO4zdn2nW#AqS2mZq");
    QTest::newRow("Whitney Houston - How Will I Know (VanHooft &amp; Forever Kid Unofficial Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=qvn0oezdDL9Ytc92v2u2ChzjAhzjEwvWvwTNrY1LoKrtCdjindzFtxn1DZyTmwrNou9ZEc9rz3zLodi4vvm4m2KVytHHC1DVCI51z2zFEtDZtKy2nxHztha/DfPLB3vwBuzuDdrjrJvPwg9OAOmVlNKZDOrICMXLl2rHltrTsOnhDdbZx25ZqMPnztLhmJDntJngqMu3vfDFvgnFEfvpyMrIBKXIAuLonZbJmxK3zs5TvgDUDJ1kwKeXEgjImc9NrM1nzfG2DhvzueOZnLPQEgyYrJaOBeP0CJmXvgmZC2qT#AqS3nZC");
    QTest::newRow("Rudimental feat. Jess Glynne, Macklemore, Dan Caplen - These Days (feat. Jess Glynne, Macklemore & Dan Caplen)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=uwLFnhq9nhDbzur0rLPIBJHZAxiXDJeXmtfctxLcCezOrfrsEvHpmZHrvtPJqZHFmIOWtgnLmNnHuZzztLjOsc8Unxrvn3GOwK1KD3aUsgDYzgnoDezhAxnYsxiVBfqVyZz3uxnXn3qODKH4yu5YzteZtNq2sgq2BxrxzhDtnO9elM5lytiZwI11wuHLlOqOCeP4rNuYEuvhuvDnz29SyOvuuNrAwtL2rMe5zhzlp1ntlxjYBKTWCLfJBJjRmxfnBwyWDLPSv2zJrwvXBtz3yuDvsZi#AqS1nJu");
    QTest::newRow("DJ Leonid Rudenko feat. Nicco - Destination") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AffxzZHUmNHotuuUsNq4C2LJlMfXEuGUzfDnztm2mhnIBhr3oxrPmxnbsZC2BxG5zY9HAI1QtZi4mZC1t2fYshbvAK5FmNbOowLlCZ1JDwq6l2fsmNiXowzZqMS4uvvoEKLTEhqTExfxyNveDu9FEdnQC2DNuujXndm5ng5czNb3zIOWshfIzgDXlZvqsKP2ANfwAtm4D1aZuJq2Dvz2B3aXtNzSovDdDdLotLHLytjUwv0OyMmZoc92CNa2ngT3yJCYDwnYq25kwKeTDJ9YsvjLotC#AqSZmdm");
    QTest::newRow("Inna - 10 minutes ") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Afj4og9WzxrTx19MownnCdbunZHjowvxlZrOEs8ZC2flDfjNnvfQzNvrwI9FDMfZDJb1CJrvAZrJEfvXwt89B2vcr1vLrv9ZCKX1utr3nwDbthCYnhH5l3vdyZPKlxvPrfreEw5RBtm1Eu5KDtfflNbRthv2wwnZlOjHxOvymf9Isg9XD25fC1vdztm2DZHjmtzoneTnnOr5r2uOA1bJogrPq2v1AteOnJqVnO9tqZmUwLLPx2nWAunitvHxDdaOndjjyJD2y3vbA3r3EvbrqM5ynZzpnZbHvZnylLPlngqZ#AqSOodq");
    QTest::newRow("Arando Marquez ft. Oana Radu - Pina Colada") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=nvLnCI5UDZvxl292yxfunhzXC2SUnJKWC3vhq2vZlOXlDwrUvdHHzwvwtMvbt1fuDeq5m2HYy29MndfjogTTqMzxng83yxvhwdLtEvfRDdnIzOHslZnZAtK3lJmTDtnmnMf6tK8VwgvyvLC2r3Prsdjcq3ndwJbhnNvewMrjzc1cltCWmdGXqxfwBdjlmZvjl3rWCw4VAty3ng9ZC3vqvNfZDhbgzZDNogG9mKvXogu4m3fLwuzPBJnIDeHsCw5TmJfiyNbqzxnzAgr4EMe5mdPZmxaZnKSTDNu5uxjhvZ9mtW#AqSOmtu");
    QTest::newRow("Anda Adam feat. Vibearena - Forever Young ") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=v3fAr2yTCKrvwI1HAs9doha1A1LuChqOzxvAudfZtwHODfbfDL9ZEfvrv2H3DdC4u3zPz2PortbZEezvtI82pOCZs1ztlMmOuNzmAt1HvNDVvMvLqLrumc5uDKHWzvjMCKm3q3zyvw1KChuXmufZow4ZEvv3AMfdyKu2mJ0Tz1HIug9SvgPWuhjYuhuVtdnfogD5nLbLqOjYB2e3CNjstI9smMvLy3rwmMHjnLvLlMv5ntjXng5SnZnXuhb5zg1UB2rXEK1buMfLC2jFxOHxr2fXneq#AqSXmZa");
    QTest::newRow("DAVID DEEJAY FEAT. AMI - MAGNETIC") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ag5Zr3n1zt1LCK5LDK9jCdCOog5bCgzTl1j1qxPlvJeYq2CXswLyyvzKAJv5AenVl2OUrMqZCZHOn1yOzwrineCZndnkzxrUyLvXm2fJqMHgoJr2qOnAzZfgrNjOzg9LzuTHsejts105y2vOoeDkzxfJAODJwc82uhGZC3rOwwnysZe3Cg53sJa3D2DPys84zOHHzgvsnKzYAeW3D1risMzqsxmYztH5mtHQmxjvAxHryLnFDKXWCNfZDs5zlMjVp3LFy2zQy196os1inhrtsgXNwdq#AqS4mtq");
    QTest::newRow("Fabio Da Lera &amp; Alenna - Kenya") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhvOyOzPnhn6mK91nJK1nu0Zn1Lyv2v4ps5VmZuYmM1ZAhi3D29LnfHoDerRn3yZsY5oCfvdAZbgAtPTotvhotHgmxDYtKuUC3Lcwti3C1DiDvveBgDWzu1FExrxB3rVzcO3l3iWCgXny2TfAwy2rwrbn3qVyOG5rfuZn3C5vhrOlZa2ouzQEfzUvMj2D2nYzw4/vKT1BZaZvKfLDvDxxZvgy2eYlwrPyxHpCfLjuOnOvc9szdLxrwr6CdrIytrZmLLiwhrHmuuVCMuVv3vuAOzHvu5YytrfCtzjz2PpBMDSuOC#AqSZmdy");
    QTest::newRow("The Sign  - Datura") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=s3GOztaZCwe5qxy4CvfWC1qVms4ZuOPSDJqTlZjXCdrVDfy5uZf1Dxn1DJbVu1yVCM5tnNLJAgfPmZyYCOvJCOnut1eXC2zjv3mWtO4Zt2O3CgfOzKPcy3j1zuncuKXMmtHXof9hzOr1sJvPr3bhmt9fm3qXn2vLCZnRDLmUqNrbnZfKl1HumfffmMXhlxqUDejrtZDZngOXz3jLoNbxANbHyNjXwdL0AgPOzerJv3rzx1DLCZmWnu1WzdzrqKrZyum5AfHyq1jinwrhztjlpvLPlZvrvOL5lZjJltnrCMrcEfru#AqS5mZq");
    QTest::newRow("Fly Project - Get Wet") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AxHmDNvMCOjOBg5Oown1uwS4l3bgyxHLA2eTAc9Ans1ryLfLAxv0shvezOWWmJHqlxP6qMu9Bu1Zyurqt3bMyOTJsejVmwHOqKOZzuDlndmWsNq6Bta4yJeYAdfWnLvpDeH3Ae1eEhfYmdqXsuKVywuUyLrPCLjKzdvOCMPOzK5AlMKOsu1WBZKXDJnfluS5rNvYr3vuwwXxDgH3DMfHBgK4qMDtCZzVD2rHnNnxpOKVDePLDJLLlL9PBJrRmxDHrNeZtgHUDwHIv3zRm3fZnOrfutK#AqS4ode");
    QTest::newRow("Jessy Matador  -  Allez Ola Ol&#233; (LazerF&#33;ne Vs. LightFirez Remix 2k12)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfbMzwu2ys9oDZbLB2G2DevZwwrZoLrLDKqVvs1ImZnJmhGXl2f4yujODdHymOvUCeLkyZfZx2SOnY9nCNq2owvQnKL1vKTKn1aWEgqYnY85BOzPutbsBNr1qL0YEMTZlJbPB2uXlJDUow5HDc9jEhDYm2PendmOzgjSCgrHB3rIrgvWz3jgBgvxovqWv2P1v3G1wvjSlwHOmMvUnxjcAMiZEMrlyZOWrxq5rZCZmxHxCZrTB191EwLgugeWsMq4zgOYCOTiCNvLytjjndDtnxmXA25Sls4/lwf2uLDltLmTmwO#AqSXnZu");
    QTest::newRow("Story Of The Year - The Antidote") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=lKmWsMHLrde2ELjKCZDflOG1t3Dwm1rwA29NzNz2mfn2B2LYCdLintnlCI1HyZu2lwGYC3vuBtz2sgr1oMvNmtvuyxnVzMz3yMSZCf01yY82DfLPuvPenhrtm2qVDhq5C3i1B1bLlMLbtgjUrY9Hvdf4owX1v3fdzgXHwe1NtMKYyw9Wrg9AmZG5DuiUofP1stGZqNvFotGZqwvrog1dsZfowvnIyt1QyxrzzJfyy1L4wJHNBZn1ofHonhj0ys8VuM1AyM1KCNv2ofHMnO9QohnXEwztuxa/uui5AuTcDez6odK#AqSYmZC");
    QTest::newRow("The Sum 41 - Still Waiting") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=nJLYod03BhnnBOzFztH0zgzfnNrkuuLbsNn2ytnPDZLXCKTPugHurY5xCuTUlKHFotzPmLa3mdj1zdnwmg1tEMe3mZf4tMn6zNiTChKVDgTWDKT2COXsz1v6r2Lhq2z1AMSTrgvcCwLKBgiXvs9WqZDHA2DQyxbkl2vPlMTss2jrlZzJyKHxzdfFC2jvC2zZEhrAvwfQveC9zJrTy2vRywSXvgHcxOfLvt9WAgLLrZrquvG4EwXkqZeWCg9OC1jNzYO4DdLICOXtDMfrB21HDgLcu3m#AqSXodC");
    QTest::newRow("Imagine Dragons - What ever It Takes") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhCUu2T3ts9jC3buqI1mqLPWlu1Ynha5DhrrBv92l2rKBMqZDNvdDePer2njodLFyMu5y3rrswjYCwmUAw03yJCUnenhmwLRlty3uvPIzv9tpwj1y2TYltjFsOrqu3nFA2TMs1n4CI9bmvP4zJnlAePHCfPOq3zOlwrTsZDinZHbvdKVn2Ttu3nStgvwzwTqmMvYAOjHtffRmtbjwNPjqLyXzgX2uJ9wx1y6DhnlChyZtfC1u29cAgOOBxrrogfXsLCYwgrUwM02AfmXC25UEvqOBW#AqS4nJi");
    QTest::newRow("Fun - We Are Young (Slider &amp; Magnit Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ag9NyvP1uhLPrtzWve44zvLVnZvRzI9dtL0TvNjdvZrJufe/BY5JDxHLlO55uM42n3y2COC3D3i5rMvbouDhys9bBw84ANzOzwi3tw5AmNfWCJjPuumVmwHeCOD1r2rrDgSWtxrzreK2C2CXyurZvZzVEefUDZjPodDmlK9HCOe6zMTuxZaZzdmXCJj6tOTOAxjWnOO1nf9TntrfrwD5ztOXD3uYzfnSuOHsBgvWwKHWBM9WnwzStdLOztDcm2T5uLCWmOD2tMzvxOvRzY14ns5VnfG#AqSYnte");
    QTest::newRow("Jasper Forks - River Flows in You") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=mLfonNnYzc84COXxDhzlxZnVsZf1yufYnNe/x3vOzfLAuvvmth0Vzuu3yL9FnLjoterKBxGOn2HZyJv6DI4ZtxjREIOOutKUodrwwf0Ou3jIzKLQsL0VlKf0mtzOBMDImdnyBuz3yuzzqs9iy2fPmLbJCdnSx3DXnJmOyunHwu5IC2KZvMuTD3HQsMy3rLbMoxbiqMXPmKfisgHvwhvsAwrWCuPQutvMlvvxmuDhpxnqoM1jsNjhvZvLDhHKBO1TAtiOywfMCwfUqLG3y3f6sMvUEG#AqSOmJC");
    QTest::newRow("Mike Candys - Last Man On Earth (Mike&#39;s Mainstage Mix) [feat. Max&#39;C]") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AejKwfK1y2nspwPVCM15mhrPxOiWEMjAs21VCc5Tmxb1AvnPAw5vmhbYl1iVAMHjDdjVzwjtmLGTzJzZwtu5yJvIrc5KDxuZowuWn3aOB1LesOCXCNuUm1LOytu4DgvvmY93mxe2odf6l28OA1bsnxniDNmWluLenxrylO9RvwTUDLLRCMvmA19fDZnMmMXVDwvcnufWsefZBxroDLfbAdbLyOXHodDAwMC4zLi/nLf2odm2rgr1Cg5zmKDHoNPereLpr1nfqun5mtu3EKjNswLwlZLZDg1xsxKZsuv4zhbJExC#AqSXnty");
    QTest::newRow("Survivor - Eye of the tiger (Ayur Tsyrenov Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdDsn1G1tfn2vwfPmM1jDs95mwzgtOG5tMCYogrtlM9kl3uTtv9VDKHmwvnFr2TZm2jLEee4rwm2Be1orfG3DvDNzNjHx3PJogT4uJfWyZqOm3rHCeXrwNbLptzNohnQDZzMndLOvxPVuNvSB3DdBKDPtJjNyY5ZvZjOs1C2uJHpzvG4rw8VDNHuCMCWnKyOmtfUzhmTywv0ode2mKv5nJ9NCwqVDNvYyLvUAxril3iTy1eZoNnRl1bKzO5zotGYtNLbwxDFn2K1Etn5lw5iBwOUrfHnDfjRnJHvywjcvfrPx3e#AqS3mtG");
    QTest::newRow("Lady Gaga vs. Snap&#33; - Poker Face Is A Dancer") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=BY9tzZHhzK5rovbZlY5Jm3bOnZbnsKT5Dhy2DLvryw9HwKPOttngEJq2owLglujHnI9rAgSVEhe6xOPSDJKZmva2mOm1ttyYtJG3BM9WndfIDtLgt3vZzxnZsOuOChrWzhDflJHYyZjInv84AvHJnMXWx2OVmen6ANfoxZDztL9rC3LcCYOUr3iZDtLRrZjYsfC4vNrLBLffwKnODtzRp301AJHXrNboytGYDffHsg1Pnwu4zMGWmwrKDgGZBdLzqv0WrNv1vZy4BgvsA1PmzfnczM5tEc1krZi5DhiVpxDLnO0#AqS4oti");
    QTest::newRow("Twisted Metal 4 - Time&#39;s Running Out (Neon City) (Cirrus)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=oeDWwJjIrgPOuOTMC2DLzgXYuhrxCY5tneTyEdmZzZHwENnZofuWmZGVtNb1AtrxutHPEvnRyxv1qxb0vd01C1f1Cw8WzunVu2qUCfrtENnHnMfWC2nHzxaVrxC/yZbXuNDKuNzHltjYx3q2zxu4ps1Qu2u1AY9yrM8WB1a1mteOBJLJy1vvvfbPDg1QwJfjvgTSCOr3nJHOEeDHmxzgDuTfl2eVm3rgA2TRzOLOAZDxqLeVyuyWC2Hkog1YDhvpwffFns10zdLUuwyUoufIzvjHnvPADJr1EhjjquDHz1i#AqSYodG");
    QTest::newRow("Roxette - Listen To Your Heart (Remix 2016)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AeuODNuZytnTzLvdDK9VDhvbvMOWCOzfmfnzzvncvfLPmdGOnJzKys56Dg9Qou84Dw1hEJrJn3jPsdH0AujltenyyxDKvhjWxZjLyxaZDNzstNHOnhnWl3jZq2nJzZyYuNe2mZHQr3n1CeX5wNa3Dt9hmw8VnwvLlxvRCMXOnNm3Dc5mzg41qJv3mOSXstGXDgPgmZHmyxaWn1rIoJnWuOrVuw4Yuwn2sc5OzgjjBeXutc1OweK3rdDknZi4lwXKl3vXr2nsm1PRzNzTAO9Ppurml3PMlZLJlO85yJzODLH4Ehq1#AqSZmdi");
    QTest::newRow("DJ Babyboi - Alive") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AwjWyMLVDKyOr2fzshfWyMHzlNrnzenklZKXzwvxmwLAte9YC2G2ttvgtxKVBgWXl1G6BOSYuMzOnKrSogK9qtDZq2LIzhzhufDmCMvRENLIneOZsxeXDJnkzejUohrHDwznsNvVAty4DevQrZDFuwD4lZHHvvi4zhrqmOTsBJfJyxHWCO9xBOfQlu1Yofb1wL9pEtruDLy3mgnhoeK4BOXHEuzcogndqJDlzOeVuNjgC2rirxf1rgq3shq2nYOVlMOYwMjtyxv1vNH6DKHSohnJthy3wJm/lM94zti2ofzoAtG#AqSZmJe");
    QTest::newRow("Madonna - (Ingo &amp; Micaele Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfLHDui5odHPwKD2v3K4yNr2nL9AxY9mAeGUoNqYCwfTDO1Tz3nvlNjjrdvzlNnQDdnHotHgzgmWsKiODwuYmwXYCdy4B1zPos8Xt3PRltiTyZz2zem3Ddn4zenwwxrVAtn3uxbTDtv3reW2EM5jyMz4mujdnfmOzf9jCLvhrhDLmtzWne5Zm1yYzv84AxvIC2i2BM9Rzwe2zu4XyuHNEfzxC2DAntO4r1vst24VDejVCZHRugf1vMOVmMfovuSVmJnmmxzOCNvQBeeZvNnfl3n5luC5mZG/Edf0mOTqow50BOy#AqS3nZq");
    QTest::newRow("Ace Frehley - Back in the New York Groove") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AgvkqJjzmMjNthnxyMuTl2TWBLbJuKj2whbTEKrXCwrVrvuYrdv2AhrhlMq5Dtn2Bu8VtwH1zwS4offZAgHTt3DfwJiVzdn4ENvut2TYyO1MBtqXvgK3p3HZyxDsuMrVDg1jEhnerfnMytaVowfim3bODgzRrtndvKvcme1UAhq2oKDzzOTxsNDKy3q4n1m5CgTKmY5ADeTIEwXev1qXmxnPDZf0mxPAz3rTmtrYCgGXl1nOBKTWwND0uNzcAtzNEwXrtg1woeH5BLzpohPqmtbWBL9ems8UnfHKzMf1lvvnptq#AqSOmJe");
    QTest::newRow("Madonna - Power of goodbye (NIghtcore Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AgjxCZ9QA2vWyw45muzlotqXCwi2CJnwCeDozMuYBNeZoM9HofDMmO5yCNH1mc1LwwS2wwnuDK5XCtf1mNjYDKfWAvvsohf6uerrztnyDgzVAgrHtMfOrhaOCumUrvPODenZsdrtweTXsJH4DvnJnvzZmZuVyZHPs2z2mvnul182CLDyqvzire8Uls5PEhuVsZrQmZfJmdyOpweVC2HzDwKTsdH2ytG2qMW2CKDxuMDTC3HiswjWzuDhnNbhzeXZrwvKsdG4oeDgr1KODc1ozgiVzgfAtJL1vgvryuzYqJzvl30#AqS3mdm");
    QTest::newRow("N-Trance - Set You Free 2k9 (Spencer and Hill radio edit)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfL5Dti4v2j2ywO3tvffoJHlwxHLBtL4lJ1UCwPtzxHvu3n1zefHCgrVu1KWzwnVofjstI94nLGVoxzLrePZsxbOmdeVtJKVp3vvBxLYBgGOy1i5qvu3BKS4zfrkCJjVDg9eB3nrAJi1yvuVndjQlNa5EgvLthvAEtuWwxbUnOi4AwjQwtbpEueWBdzdzuuTDMeXm1vHzuTKC3rFwe44mMvcsMW3sgvLuxqXAKfYzZfFx2eUlu5pA3m5ofL5AtHSAvPjtc95uuDtD1LnzL04nwm2lxjdzdrKy2v4Awf1C2rou3q#AqS4nZq");
    QTest::newRow("DJ Smash - Moscow Never Sleeps (Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=lOzumunJChrkDda4CgHJu2W4B1vflxHoBOT6zxrIDxnKsJvOwNrRDMLTAxi9Dc5VrKXWrxz2CY9Ltc5TBLv1DNLrqKWVyLnACuX1yKDYl2m2yZu5BMWOowzQsZvrs2fVp2HSAwyZnKvKyuKVogf6lJzIqZyXnNu3rMTiBO9UyMm4oJmOt19smMDrndn5wxKZChDumZq3zvKWq2vSCJHXDLuZqZDxDu13oxncuuy4zgDJC3r1C2TmyuuYmwnYAtHIvgXimw1dq21lCMzmvwTzmOnYt1b5nZyVv3q5qMf1uLLKogK#AqSOmJy");
    QTest::newRow("Plazma - One Life (remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhnTDgv0zfDJndLNs2TwmM1HyJDOovPRAtHfzNDKs25Yq3bzm3zkDJLzzeHHlwLwtwD5nZL2sKLOnLLOCgCOAvuOlY51EI9Yzt0XyLnJm1e4n2jPBtbFEcO5CI1KyK9gnY9iC2vvzJ1MB2rZCZzvwhaYzxGXzMfHlY4Xnwf2EhbqDOeWDhnYrZiVz29wtdi1nhrTue5TDuXdlJzHwtqOngqXmdLIuZnOBgjfl3rADxnuAO85AxHHtZvMq3Pey2qOmhz5mg5RA2PTu2TxEuXux3nRsgHWAMv1tNz0AJ91rdu#AqS4odG");
    QTest::newRow("Breathe Carolina &amp; Dropgun Ft. Kaleena Zanders - Rhythm Is A Dancer (Extended Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfPMseHWtMzLmhvTvfbNm3b6DMrlmeeZwJDLvweUtZfJnOO2wNn4tNnVodjOBunTpxHXthbIn21vytzxAY5MCfqXnZnjm1nOEM9WzgK2t2zUlZvZyK5nthbYCe5bqwTWtwnAEhrFotz1EKHUCfq1Edvlu3D1l2qTodjolMDPuMTSyJfnAwvLBwvIsJfkB2uWzMjquM1yC1fLAdrAAdv1ztPOBJiOzhmVwMLNBODLytjgv3DlqNzXm1PHCNLYwMTWlZf5sKjzDsO1sg5vEt9WDeW2Evi#AqS2nJC");
    QTest::newRow("Pakito - Living on Video (Ayur Tsyrenov Remix) (Radio Edit)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AgjnnLvduwC9EwfhlKHXsK1VocO4yviYoxrYre50yLjZA3rtthnSCZ0OmMfYns9IBKKZDgOVDxaWwvDKtxr2y2LYEvz2m1iXufPIveTxAwL5ywrsndrqsJrPogrKEMv6BZnTC3rPywf5Dvj1ngLUCMrYwwD2rfHJmMuVlLzUnun2y181ndzqzgH4swvVDe1ht2vHCvmVn3bTBg9ImLjHzwmXB3PgyOqWwM43qY5LoxaZqMzZDvvmDOPIzLnHsd9JrgvLCevODJr2ChrVzs14tfbXCgy#AqSXndC");
    QTest::newRow("Alle Farben - Bad Ideas (Mikis Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=BZa5zwXPmgrYvgKVCOfJuu1Ktezxqtr5wMKVlY4XDNG5puPhC3feBwnrwdrlB1bizxftA2HTCgGOsw45ywHuAMrXotbbnKDrnvLJme5wsIOUmwngveXHC3y1mtfHCNbVyZG2u1L4u25LoufIndmYAxriDhvbAtDPrJzZBMv4yuKYzuOXCwrZl2HSqvfNnu84Dgi4mfLODMvOmt0VDfbynv05BLfhtgnvogPoCdvYp1qUEfD3EwrHzhHhEMfRy2jhqtDWzfHeAhv2D1LTEfn6oeeZAMneEtGYmw14uNvqAwGWl3vg#AqS3mJK");
    QTest::newRow("P.Diddy ft. Dirty Money &amp; Skylar Grey - I&#39;m Coming Home") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdnFnOPsDKz1DeL5ndiODfPusKjiDLvHlOrYyurRqOSXq2r2uwr5BZDRAuPdu3zss1K5rw5FmdbTDdq2t20ZzfjYsd9SzMy3qtrvzvDRq1G3oxD1DhrYlMrbAfHpBhnWAY9kyZv4Ac8OnfuUstnhneTHue13wLKYEg93wdvsqOfQmtuVEJKUmhnWmvHnztq5vwvYAfaXnZfkotDLzva2yuKXuZLFuLvYzgi6mxnMC29ylvffptDRzgjImxq4mO5QCMPRuxr2CfbtveTouhrWDu0ZtW#AqS3otq");
    QTest::newRow("Pendulum - Propane Nightmares (original)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdbQvhe2ngK/m2X2D1rHCMXroxuTsxaUxZfODNe9zNDwB1Lewxy2ywqUruvjrhnhDhHLuwTovZHxEwf4EMHTmZrpANn5mKLABM9PohP1stmXltzZtgfYDNboEdDOoI1RDwnUrY4TAZryrMKVwhnLl3zJzxvwrJvesNP2D3a4x3bOBgngC3Hbtxv1DdzxrY9Vmuy5nOrknZK2uLH6tZGXuwXNlO5iEdjYq25hEvLnq3nhm2npB2nModjJngnwwK14mhfhqY1wrhq5zuHVv1D6t1vYyxK#AqSXmdaW");
    QTest::newRow("Shawn Mendes - Treat You Better Mark Ianni Bootleg") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfPYmuHRsxv0ts9UBO1cAZfWENbyrM85CgCYzwLSCZy4s2nqzwvVmxuODJDwotHAAfvrntfjmZDNCMKOwtzKDhLZmwLSD2rsCtCVr3viEfuZu1zSDhK2n2rFDtHelNDbl1GOm3fFyZHptMWZy24OCZz5lO9rtt05CJLUnMyUt1zMpZbLCdHKlNH6lO12ytrcutK4zfvLAwXKqMf1DwrABKn1ohzPvO1PmY89t2OZyZzLnevSztHtyMeZEueYvvvcoe9yDwy5AfPHDJi3v2rVohbZswHKCKTYCJK4mwDODgDTCOC#AqS4ntm");
    QTest::newRow("The Prodigy - Serial Thrilla") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ae1RC2P1mdzOAvfxuZzLvdvnCeT2tvbYAeeOnvbhshHIt21Oue5oyxLynwCUCOrVsuvKyxbnt3qZwfrSlZ9ZwdrunwnYlOvOChm4rLL4CdDFrgfSshj1lO8YwJnHr24Xrtzesc8YCwzwzwXRquu4EI5OoKLHufPQwKTWnMrPy3PRvKDfneW3mxr5yu1UrvzqsI49A3rJmeDfBZfVBdbAyuvdnZr2vZj2r2CZzujKCNbvmZyZzv9wm21dn2XJDe5vmwzhmc1LmtLYoujItLm5Bg5vBW#AqSOnty");
    QTest::newRow("Moby - Body Rock") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhmZlJLNy2fpuu9LqK02t3vToNGWmNbLzxfAA1LsAK9Xt2XPBwK2mOzREJz1ngnOlOy5rtrYDfPPsuHWENPYnZH2zeL1nNeYmKCVmdzJvdaOAwr6mZjOnY5jtJeZBwTpBI5esZ1joxLPltrwzMrOtJjQChaZENHpAeCWzJbOlxnvDva3zxPlAgviCdGTtOHUnJfLnvvjDK56zwTQy1LmC21JAOGTwJjZrtq3x3zHweXUne85whjmmOXLtxDJBZyOpOH0m1D6lOeXqLz2BxaWlOP3yG#AqSZnZm");
    QTest::newRow("Axwell - Barricade (Radio Edit)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AgT1lMiZs2DZvdv2v1fZpwmYlxvOy1jJqxnIBMn5nwuZq2eTDfr6zePzrLqUmZvhBMvOvNvXAs8WDJDMAZ9cq3nTDK5Yl1fnDNm3ANb4CdbFr2iVEwPkl203mdnHqNm3CNbYltzjrv0WzJrQAfvNwMrOoNe1EhbqDKrgyNq1y2u5AtqWztD4yvn6ofvVmLr4ts4OuOm4Du1OBxrzBtrkt2Dlqtj2sKrLrZrTzw5IDhbUtgCYohnMt21luePOzJfOnOiOzeeTmtv0s1vcvgvPsOPKEq#AqS2ody");
    QTest::newRow("Zara Larsson - Uncover (Richello Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdLxq3jny2zOAtjUyMvLuJreCOnwCJHYCfeOAxyWzuflBLnRExj1ywfOstuUqOvUB3jHvLfszhq4stHFlZ9ZDwnbowfYlOzOBxnxvI14CdbvA29KAvr1l19LAJnHDu5SrdGOts9vofb6q2LsoweTEc5UoM5imKjcrfz4BgrqyZbNmZzrnLbFmeXinOrUBLmOrc49y2rUnhK2B3ver2rWmMHJs2n2ow52yvvuzxjHBNblsNDQogPynw1youvfnOflvMTUuY1LmtHOtfvzDLfStJyWAG#AqSYmJG");
    QTest::newRow("Muse - Starlight") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Bf9huMXJogzWmvf1C3a/m3iOl3rOl2LYuOLPzJvAuI5JDJmOCgLiss9Qn2vSoePqwujPl2zOBNjHC1iYoI1JmeXVmu1YtMG4uhbkDerKluCOyvj2C2PoBMHTtNj5nxfLugfIvfi2C3uWEfHOEdreww9eDK1ltgT6y3HitgvPDJHuzenxCeqOwMXYqw4WqI43zgrNwgvhmxnyAK9JAdHFAdaUodeYD2K5mvGYnNKYtLHSq2qYBxG9ogT5vZvMrvbtn3npzKHNy2jOmw5hChrwy1nSCJy#AqS4otq");
    QTest::newRow("Of Monsters and Men - Little Talks") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AffyvvPZyY1OAvfHogjLr3mXCZnKtdDYCgCOngDltxfvm3nRweX1yxrOzNiUDOXpsdC4CO9FAxqOtxnylZ9ZswXezLjYlO1OvxnLAen4CdLzywDAnhD1lOOYxZnHAeXfqwq4tY93B21REMvTqKXWCY5qoNzZCM9rnfzRq2rQyOLZrKfQmuTMnLLQnfzUDLv1uc49wfe3utzLB1Prs2r4ogTIugX2rxv2meDYzxi1tNbqzwPHv1btngO2oc1rsZfWBeLtCs1LmtrQqNHRCJrtqujroq#AqS4ndK");
    QTest::newRow("Darius and Finlay  - Tropicali (Radio Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=C1z1DgntBOWUAgvLsunYyMroAZrPuKS2mKqXlY9Pq3ncCOjYBNy1oeyUBNaVsfbtovHLngnWuKuWzfHOCIO4rOveyKTPoejTAvvhsMm1Ew9XDe1JDc5VoKTZr1zLss1VwweYEL0ZyODsrtbMCNvQyM9poxbfqxH1EwTomdHOnxKZDNLgouTKAguXvtL1rNzHvdfmmY1Os1PfnOi1AxzhzZrFzNG3ngWZvOm3A3rHytfLmhfrmt1YnJjLmZbjrOnhpOTUqI1AC2f4DMzymhbSl1CZsa#AqSZodC");
    QTest::newRow("Kenny Loggins - Footloose") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfiVu3rhufK2CZi1vJLiAvy2ntnUogX4rhj2qOC/yxC6tgnOCO5Om1jwyvuOy1iXlwS3nLnnz2e1qwjZB3vKl3P6yw5lEhbWnemWthbHms5SA3LgDwvezhrznLLlmfHothC2B3rgnNj1nwjpy3vdCOiVAeHYDNbJx302nwDVmJfqwvnxzuL2AxaVnee1pw1krePoDw9WCI5Zm3vkvZyYzvzplY5omxrYx2jimKXMvtKVnJfwnZrAnNHOvxzcCvvnCvaWEwDHnwjYuu1QmufQwc14zKjFntyOzxaTmMT1rZbV#AqSZndm");
    QTest::newRow("Katrina and The Waves - I&#39;m Walking On Sunshine") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhnOl2nrtgvLBZu5CeXPytfftwDznvPMnMeTnKz6DuqYsOzPmNLKmZHRmZfNmKjvzvC4Bwq2nfDODc9OsOy2Awe4CZGVzhrIz29Zsdy4vg5XtKvUys1cmY4Tvc5MuNDcrhzZtd10vdn1vu9gEhnJvOjZzxbiz3G3qMTQqZfOyu1umY5iqOi5BxuZCeLsq2HQm3v5CNvpCu9MELfdogTSsgqOqw84CxzUEhbRsLHHtZvWuwHpDhj3ve1LDKGYBZL6pZGYqLKTCMeWugz4CNyTlZP0sG#AqSZnZK");
    QTest::newRow("Sia - Never Give Up") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdDYpZjOovCUAs5zrMvFAxrJyvzVBwDKAhzluhbuzJzHA1yXzY4YCgHLCOO3uM5fqOS2zMi2vuLVAZuYngLZDwLUwuz1qxrOzMWOB3jlDJfcl2nWrwfWrejmnePjCNvfnhH3rtPTtuXxAKXxmIOWzwnpB1fQDxnhzMvsyvfMq1bTAY9uve1HzgDiCZfNzeTMvg8XrdvOlvHTztrpy2u5ztOXr3rXzNrkDwGZn1vLqZHWl3DXq3uVAheWCJf6m2TyvtDPluy4AJDFDJDMEgL4zJnAs1e#AqSYntu");
    QTest::newRow("DJane HouseKat feat. Rameez - All The Time (Radio Edit)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ag8UDw1WyMiZuK5kCZjNv1PqA2vjqZv6DhCVltnQy1qOEMvgCeDRCeDLAOjMCODWx2rWugfvCffklMX5DhnrEgSXAxq2m2DMzxzIDL9onxvMoKL2DtzvnJbNmNjxyxzLAg1Mrxb5C2W1lxHOnxrQyM84uO5VzI9QlxGZsMq2CMiOtwnWtxD4oxv4z1bFxY8XlO1kvgmOBw9YtZf5AuPYve9VluW3ogCWqxrwBKrSnvi9C2OYttznCtzUswnksemYp2fAr19lnxe2tJaUywfmzNu2ogK#AqSXmdi");
    QTest::newRow("Juloboy Feat Valentina - What A Feeling (Original Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdnRtvq4yuH3l2nyx3jwm10UotDinZjsALn1C29Jt25VBLrkDhnxEKPQuxjlrKDPvNzLDMTqzs1jAc4OsZrTBtrox3nuntDQBMXomtriAJe1lwLOmtb0txb2CdCOoI9KAwvLxY5tytvIm3LLAfb6DhvInKrWohjzugvnBhaXrMi1uxvKtJfewM91DhrOqMLIuhDquNLYogGWAY13uZKZuxPNu1LnsJq4sufXzhi5otnwnJiOuO4VBOO/wdmXueL4D2vNyNL5EJG1DwfIEMronJu9lZG#AqSOmZi");
    QTest::newRow("Dj Sanny J feat. Ice Mc - Move It (FernandoMuzax Balkan Club Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhLVzujLBgm1nMSOwge1m2LAyOzpsI5FmenuEMuZz2K2ltjnmKnZDxvUytzwDJr2yuHLq2frndeYufbjoJjgDhHMC2m5n3vQztf0y2e4wNzImva1uhfHCe9bzZz0Dwm3qM5KEtfTBdjWy1fLmvfHr2e5BLLTlNH0Dtj1C1PkDNfKoe50xY9UogqVvf9REuvmnNrWwxnLq1LiEu9Kzve3m3K5ELfJtO49ucOUt2jizNz2l2DHzJaVEfPIzNjIzwqVqY9bDgHVBO5juNHNBujiCNntp3q2BfLcmMr0DZngn1r1C1Pf#AqS3mJC");
    QTest::newRow("Jennifer Lopez -  Ain&#39;t Your Mama (Sasha Vector Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=vc8YDgm6oeC1CffQuwnHnJ1Pn2rvDc1qyw1NAdHMDMHdDxqVzxaVntjLC2TFndCVqwKWBMf1EhvLDM5ZvMK3m3fnENjKztfLCwfttg5KmgrOnxnNEtz0BtnWBgCZAdDVmtDWlZLTvJn4AeTbotbhxZG2sxGUmdfMCZG3l1LJq3iOzfLtB3voB1uZDxu2DfvsnhvxCc1ZzgjWlJ9SCOXZvMXnq2PJyw9XyvbsywjenhjpBeP3lvL3vei2tK5LB3rVnda4nxr2AZjrztriB1LmzM5mwe1vBZmUv3PZnOfgusO3CJfl#AqSZnty");
    QTest::newRow("Rob Thomas - Pieces (Zilverstep Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AgjwyxbYBZvHzhaWmwyOCOzRthKTvevVwveUvxrcl24YmvrkyLfgmJrmsNHpCZjSsNy1yxaOmLv1uxncCZKVm21VmveXpt9ZoNGXzwXHrLfOwhbrrdDgztbrAxndnLbRy3bOl1z2Ewf1qxfQztyWBKzPBLC1nteYDdzil1bMDtrcyMLKseO4DuCYmNu4vKjArLv5svDlzhz2z2vwDfuVyuvky1CXzhPKEgyVlLaZzhi5C3HluvjfAtrcrtjSyufQt1jhzw5tAZzpu3LpzY4WnM5hudy4Cunxs21bDtLomvbyDufT#AqSZmdq");
    QTest::newRow("M&#216; - Final Song (Droks Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=BuXRzgnYCJe3Eha5z3bJCf9bv3rrotndlZnWzwv5B2L1DejuogG3DdvJzMKVnfnpl186B2D1ws5Ooh03mgK9D2eZBJmOsZrIENztnwvRqNr6nfvOrxmYmdm1zfDUExrglwvqqxvMDxC1qZncwMnOyuHZAOzHBxmOzhzwuJqXBweXytzWBeC1t1zxu3D0nMj1tdnLstDun3eOm1bfouyWB3bHuhfjnJLbq3PLouOVudfiCZyVus11DMjWCgC1wg8VlMOYBJn5yxvsmwjLANHiohnvDKLWugu/lNvRowGWtwv6xZa#AqS4nJi");
    QTest::newRow("Alan Walker - Faded (Remix) & Shuffle Dance") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdfguZfPDu9OrxrHCuHZthGWCZLQvKLNENHVEgLmy21ZqO5UuLCVAhnemY5OA2zoDePIBI5FCMrWlOK6tJvktwnUywT6nJbjAv8XzvjRs3zsyufJsgfSm3vzotnWwuf2BJneDt9HsML1x1r3AI1pzLjMu2zTvtfxtOfqyY5WA1LRDvb2EfnLtdLMmvK4EK11uOGTrY1LndKVEgO5uwHAoeHxDdK2AtjzttDfow9HztrYlu5ZuevRzha9lvLpEwrnEhiVyM9bB1DIDtDpyJqOzZH6uMO#AqSXodG");
    QTest::newRow("The Pretenders - Goodbye ") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AffJyOfJrK9Pmdbhm3vlowDOCe52CeHVx3C4lwLjys9MztrXCY9YmwnIsJrKAwvfos1VEwOYngi1mvbiyOnVDgy/nJPzrhuUmZfrsY82De44vdvWv2yUANDYx2jRqNu2C204Dgy1vJD0rMrQrvnZytLktde2EffFre5xourolMO3yO1tvgHiC2vPmxmTzNnSutvLwMPmng84uJu9CuX2zfb5ywriBhbrmeuVz3rHBMfYnJiZEJK4nwvLAMSXB3fZqxzMqwvOC2fODLHYDxbcwJmTuW#AqS5nte");
    QTest::newRow("Alexandra &amp; Stan &amp; INNA - We Wanna (Ft. Daddy Yankee) [Alex Molla DJ &amp; Malanga Vs Garofalo Club Extended Remix]") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=mZz6Cg9RnLLux25RAw82uO1AD1nlnveYlZHZwLrFy3bRqOrJyJblzwX5lLfZmwvWutHoDg5OyJryuZf2zdbTsI8XnNH2ugWTyxnlC2vxmwPbEhmVyu04yuXWyvf1qMyVnNbLnJP1AtLZC2LFuOztAs5evgDSDNvzx2O2sKvbuwW4CLvWpOm1vdHeC2DYohP6twjomey4CMv6yKeYDOjsyLj1odneAfbLzg9WzeD2ogfzx2eZl2zhC1nsq2r2AKr1uuLoDfy9ueW4lOi4DgLbDOS4DOKUtKTwvZrNnwn4Awq5zdy#AqSOntm");
    QTest::newRow("Icona Pop - All Night") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhvUne1jp3nUtwrLAxDdrJaXywjKnxrYl21OqM4Upw9hAI50rhrcDdDbBMnqm2Pfzgu1oxzRzKW1z2fYDwnMn2fKEgKTA1fHAtjMwg5Ou19kqwL1zunYqMzwndi4z29KAOLJDK14zg1WovbTrdm2BejHChrXox01BtHftZqTBNbtyZKVlM1frNnbAunoztvAogvYwM5JngHAEwj4vMrRDtHhnujpAxKWENi6yNnSCf9mx1riD2q1wgeVuhq4mfPmyxH6rgvlmw1LuLbAm1jLl3LOBW#AqSZota");
    QTest::newRow("Ed Sheeran &amp; Katy Perry &amp; Nicki Minaj &amp; Justin Bieber &amp; Sia - Faded (The Megamix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ag1ZB3ryvvG1Cc9lrOTznwnLChu2rgi2tJzqx2LdsJnZANbUwKeVC2nYBc5hzhPwDeDOtc5NCMrWlZy6BwHJoefryxfdDZKZrgrJDNrKBwfryufJtvDVmOrZn1Dzy3z2ttaZtJ82rJrdt1rdBOvwqwXKv3r3ltfbuvjyBc5WwvHTDtzNnJLLuZvMndnLD2vZBJqVBOXLsKvSmviZzfHHDfPjDdL1suL6Egy2mxbcqKvSlvrLC2DRywy9z2XdBvCZEhj1Den0DJCXDtzZDhzbmwfHz1C#AqSYnZy");
    QTest::newRow("Nick Kamarera &amp; Alinka - Get A Life (Mama Yette) (Radio Edit)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ag1TshqWyLLZswrhy3jRutjZtgnADePgDuL1ovn6uw5VshrwDJv2xODblMCXzwX2ms5KtJKYzeTLx2iZxOH2uZKTwfqVExL4l3vcEejYmvGWogeWz30Op3H4u3feqtvVDenWChn3CwS1yxiVm2fNmxbFDgr0wtK3owHemfrUCw0YoNaYDeTyzwnqnKvwtdfJuMvFm19LzviWA2PYBta2y3nTudninhjIvfrcBZnYowfpl2TtDZzlourftv9SAtn1BZrXDwLgDhnRxOrKl1aXmwPhlwTmyY8UmOrcEgf1rfD5ptu#AqSOnJe");
    QTest::newRow("Ace of Base - The Sign (Necola Remix 2016)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdnTuOPKnwvZEMrlt21utM1ZsJHsDdjADtv1EeTWuwPVAxr6Dtz2nfDTlKHnztD2DY5KyLyXq2zYCMnSzvy4uJfyDuCVvLj4l3uYn19YyODTtva5muuWpOvwywDHD2nVDeu3ChnHALCYyuqVmMeWnNb4Dgu4DZzkowLhmLjUote4oNbwovHxtu02n2vmvOHJDhG5m2jLzxrkmO9JCfe4yOLumJmZwvu4EgfRBtHYmLu2lO5nu2rAzZb2m2qOAtGZDtrKogLbq3nRr1PNlOj4mwL4nLHtnc8UnfP4mMf1DK1lptG#AqS1mtK");
    QTest::newRow("Sia - The Greatest (feat. Kendrick Lamar)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhCVytjZDxq1zge1uMvsvg53ng4ZuhP1BtDAzdbLwePVnxnLAZzVDMXYttbHCJvhnte1yKPWx2y4tOeTAMzRl21nmfzPmuTMqI9hzdiZDsOOAenHvM9VoKDIC2LOos1mzxHKwuyZlLi3wtiUognvv2TADxbZtxHcEI5PnMnOnMzoCKHWEfPUxOzuCJvlBwvqwhGXAxb6ouD0tu0YCuvfquTowtzrwwGWz2PQzxrhEevdDgXYrhjHyOjipxrwCZaZpOzIDMOWngfUtvzzENbNlOXNmG#AqS3ndu");
    QTest::newRow("Jet - Are you gonna be my girl") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ag5hBdfOyOnTwMvUwv9iqxrZsxrgq2DWytn6CgTZBZrLzKPmDxnQqMPstvfprwzNm3zLAwTOttbjCc5ouuvTodrkANz3nNqWBJnmEejZtZj5lwrZmtqTCxbeytC1oJrtyO5LlI5cBdrpBhuVEvqVDfLYzLbbDwvwqNjYEw85zO9pvc9csZvzCff1EMP1D2LMnMHwtgHtsevyBf9iDefIrdbkAgzRoc1hogngx1D2rwqToxDgyMuVyLy/CtjKotH4ud1kqNaWvdq2zMfInuzxze5YBM4#AqS5nJa");
    QTest::newRow("Beyonce - If I Were A Boy (Saffy & Frayz Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AgPTqMiXsdHZm2rvsePuEKfZzMjIDePoDwX1mJHPqxLVv3qTAJz2vxe5lK5JztH2mc5KwJyXyO9cC2nLy1jHwLrMCuuVnOD4l3vLEJLYoujjywKYENm5p1zov2nyAtbVDe1YChnlB1mZyuqVowfQoxbJDdfqDdreAKPjmuDUyJa3oNbpDOH3zJq4odbYr2eOD2PVm1PLzu5HwhjfluKWyO5gAJngAOXmBe1XnZrYCLPjl3H6yMvHr3nWzffRAtvfuZrUtwLllxnRt1e3l19zmtC1Dvm5ys8UmODrvwf1v3KYptu#AqS1nJq");
    QTest::newRow("Mike Perry feat Shy Martin - The Ocean (Denis First &amp; Reznikov Remix) ") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=y3ryoxDMEvz2Ag8YmfjuqJCVlMy9mxrLBLvLAxbkEI9MsJzOnwj4AeTnC2XeCgrdrtfdt242ywHTDgvSnMiUmJLMr1bjr1vnvdq2nOTMltuXwtD1zOLYDMrXEu55EhnLA2zwDZn4Bs8Olv8TqZnNwxrHD2XbDxHVndHNttn0zZrbBgnJDMz0p3mTmOHWzwe4z2vYyw1VDe1zn1fAvMyOovr2r3z0vtzhteO6CNndsJrku2S2rdbHDta3DtbwlMLznM1dsgj4m2nPDdnKwxrWl3bymq#AqSXnti");
    QTest::newRow("Major Lazer feat. Elliphant &amp; Jovi Rockwell - Too Original (Dj Denis Rublev Bootleg)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AduZofy4CgrJm2eZAuXtwwnbm254Cs8UqMXHlNn1vtDrotuWDtaUwM81zdHLsvv5mtP1DdeOAvL6vMDZmNjYtxvLmNbIowS1DdLdtxLswteVmvzSzdi/zgr3DLHqEJvTyJnqDKv6AxnfDwS3B3aVyxv1nZHqtvqOswrzDNHODhDuBfvWCxj3DMLkC2yVoeHbtefhBhPQzha3CMvFmKP1yZuWm3LYnt1ctZaOx18VBOr4CZuZB2nOyJuZz3a3ywiYl2O2mhrVmLfLExnjmMH2nJzyx3P1n2K4AtfHAw5vEuyZovC#AqSXnJi");
    QTest::newRow("Tim Wynn - Rock And Awe (Red Alert 3 OST)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ahy2yLHHyxLpl2nJwNj0uZiUzxP1ztnhAKvIC29grvjcAO0WDhn4DezmtgnozO1FohzLDMTkBg5byI52tOTTDdrxwNnJn1jKBLPREfvfrtfWlwiOowzOvhbuCdz3oI9HwffLtc5iAdzvrvmWvgT5DhveoxmOzNjFytHhx3aXEgrdEI1csffFAvj1vM12z2KYx2fNog1loc1iu1nwzZK2m2KTExjUsLLnBNbQzgzWvdnozhfOqZqVnNq/swfXsvz4mwvdBKX1ogq2DMfKse55suK9l3i#AqS4mZy");
    QTest::newRow("David Guetta - The World Is Mine (Original Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ad1hnfiVrdy3mgO5whP4A2XZCKrxsuz0vhrTl3n1CdHLD1nYBNzxofiUusO2BLvYuK1LyMnWnOCTutnlAOnfyZzdmfaWmuPPlKzmAwO2u3fysfH4utjVoKS5nLHksYOToxK5lLaZALvTq1vmDhjdmvzNqxbOlxHtB2DbDwvOyO9lztvVCOLXCg5qsvfUmem2zNzwvwHgqNvJywvKAKj4uvHxwLHODJD1uuq4nhrHDhzzDJryzxiVzeDLvNHcEgCZp1zdx3LYC2foAJjOEhbAlZnmlq#AqS3nJC");
    QTest::newRow("The Calling - Wherenever You Will Go") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfDOqwfKnMPRExr6wNrotxG4zdHqoefwDNzRswLNy21ZrdfUu20VvxnRCI5FufjbDgLYrs5YCMrWlZu6rtvNDMfdyuf3swz2qumZzwfOq3y1yufJodCOmZb0ytjtuu52DhfWwd9Jz18Trtm1mdeWnLeWDZHfmtfRsKHOAY5WuvzUDtnVsKjLzdLyzwm2rxfpEwDFnOHLttiVqZbJude4BOjgDgvgA2q4vxGOow9muZK1lvDZn1PRmNa9s3u2D2vrEhiVB3rnuNG2DtzYD1uOmtvinLG#AqSYnJa");
    QTest::newRow("Sia feat. Sean Paul - Cheap Thrills (Shoco Naid Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Af0ZlNq5qOKZmhfVCMTKwxrPogrIn2zbrc1Vowfpzgv1wdLPtg53A3bwlZKVqZvgDdD3sZzns1vQyI1ozMS5AwqZEs5KDxvhmZjzDtuOAvDVnv0XCNuUzuzyytK4nv9oDc9Iog82oezTl3HvCNnXofPPoxm2CNzNmve1l3j1AfbnDNbzC2viA2KOmdHpCOKOs2vVzgLOyNm9Exr0nK1ttgnLC3zHy1fLtKrrnvy/C2fcBdnwtZy6Ce1RmvzHstD4BNzutvfLutfuDtK2u2HVmu1Il3fZAg1OCJK3nKX4ze1Jz2S#AqS4ody");
    QTest::newRow("Hardwell ft Amba Shepherd - Apollo (Radio Edit)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ahn6Deu9thfIDg9nt3vRDhnPCfCXn19IvwfcsvzYy2nKzuOYx3nZu3rkEe1Wr3jIAxzLv2TgzxzHDI5zsg5TuZrjlxn3nYO5BJvPvgX5vLfIluzdnJrpDhbWCdr2oLG3B1GVDY50EMjMmgmVsI16mOjxywvrAMuVBMnlnfrvnuXowgHwCKfAAc1QnxD6mMK4nwfSzLf6wNvSCOjUttzKtZDxoeXdlwnhtvD5ze5FqtnPodjwqI5nlum/xOT1m2f4n3y5EhvxutG5sMf0nvCVvZfYuKW#AqSXoda");
    QTest::newRow("Adele - Hello (DJ PitkiN Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=lureteTorJCYmJvMq1PJwtiOmxr5C2fAlZLAzwvXB2LilvHzBwH5qJnJB2iVuuDwlZe6B3Lpwc5OnMHpogK9D2f1DsOOy1zUoveWCMvRzw5ynxn4Cf0XnZnTzejUmxryrZvSshvyrJK4wtjpEw5rzLL4mMTWAdK4zda2lxrMDs85yvDWrxP4C1rXDI1Sog91sJyYvOnSzwK4yLnIoeeWsfDHmNCWmgfnthncvZKVC3jqCZDZq291wdnpCdC2n24VlMOZltb2yxvVmK85yLKOnNnvDK56Dhi/lM1YC3jXsNvWuM4#AqSYotm");
    QTest::newRow("Marc Korn &amp; Clubraiders feat. Orry Jackson - Everybody Likes To Party (Radio Mix)") << QByteArray("");
    QTest::newRow("Lumidee vs Fatman Scoop - Dance (Original 2013 Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=BMvpmOzQm2HpsZyZudnsAxrLy2SXsO9oChLmCs9MCdfnywmOCw1HBgeZnOz1ytztDhvQzwHYwM9RDNbenc1TAs9vx29PCxvInLPFwhz3DgeZtNKYl3vZmdDklNHhDhzLmde5zL0VEwrFmKnymLHODJG4BhrfzMzUnwD1z19Zuuu6uJncqM9gzfq1vZHFEtz0me4UtOSVnezymY9QC3uTn20XvZHKy3z6rgK9CKvbutnJzx05Bf8XsJq4stvwCLbOn3y1uI5cC2Tuz2r4vJrMneLZmZHKzfndyMvqyxjXAJ9sma#AqSZnde");
    QTest::newRow("Sia - Unstoppable (DJ Amice Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ae5UDOm9DI84zM0YmwLfrLzJnLzKAM83COrkrwvylL0YoM9IodDwmuLruxPOvwPLA1q2wtLADJzKrhj1nta2lvDWAtHHof9Rz1LVrtnvDc1VzMrUr2fOsMSOsMHlDtvODe9Nwhe4BZvsC3rkDvq1neK/vM0VwJHPn3n2ugXLl2TsCKn6DY1smLfMBs5QsL8Vu2vJmc1Jm2K1mweVC2HHt3bovJHWytyOn3vMCMvQlNrTC3G2r3zWn2n2zgfizgrfALrHru44xOXdyLC3CZzQttDLzdfzmdL1uOj1lwLOwJzglZK#AqS1ndK");
    QTest::newRow("2-4 Grooves - Writing On The Wall (Original Club Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=CZDTCg14l2G3qwTABKnRrZGVlKK9t3rjvZfRAxbUqY9xvu1OrxzJAffpmJnfzgqXsLrOutH1xOW2qMrcB3q5u21qyY15zvHgDtGZsgPhwNKWqJj1rfnYEwvcDdbJDNnLAODHDJLWuc8OnNe4ntnKmwfHugjWDxnxqJHQCJLHrMm4qMj2mvLVp3nxm2vKzwfgz2vYwgv5zOeTmxfYlu16mg83uxzqltbcwxm6lxmUq1DkD3jwtO5Xrdm3qJzWtZf5mLPTDu9qCZnQDdzZuxrWlMLdBW#AqSZnJy");
    QTest::newRow("Red Hot Chili Peppers - Snow (Hey Oh)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AfnhstDJnZrKBNuUrfvmEuqOzY15ns1ODhaZoxrOvO9Knhb3CNvzCdrrEc1HBwK5vtGVteX2murTruu5ys5nmeG2mJjnAgCTswrLrOy4n3vUoLv2DfmOAs9Yr3i5nMHLx21LrKT4CZGZn3rkmxPnz29ZDvjVtvzTnhGZzufFEJDfAtDUzejhx1LnA2fenc9px1mYrgzXlxbYnOnQnK5UrKOXyMnHxZe3revus3Hgnem9C3iUqL0XAJnuDJDgve1op2Lrzs8WwM9LtxmXCdfQrNjyDYO#AqSOotq");
    QTest::newRow("Lisa Miskovsky - Still Alive Teddybears Remix") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AgjrvY5ovgzZC2L3tJC6v30Vuhq4ys1LCZGVDNaXyxjcn3jsrOXbEhf3zunPzg5tB2G5Cw53lY41zxrImgPVs2vKqMz3mLPOyZrSn2OUn2r1mLy4nJj2sLbhy2XMDM11Ede4yvzAl1D1A3u1Eha9qxDLEhnFDOvvrJHHC2nKytrRAwLRD2zFlwGTmg9onMnWExrVmgSXCOnfp3z0zNv5mduXrJvnzty1rJHeyxvonKn6tLvPterutLG4u3jKCx04Bs8VDe5qmJHwCvvMm3LcAu1HCNrLBhHomZjSzNjLyKfZsgDX#AqS1mZK");
    QTest::newRow("Carlprit ft. Jaicko - Only Gets Better (Radio Edit)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AhnYA2TyEhHKrJfHmwzZC1n1Ce4Ytdndl1GOtMSVmguVrNf2nZrcyw9kBMv1x3vQz2OYC3n2vc9lwhfpENjjDM0ZmhbSndDTmwu4os4ZtJrnngvWzdi4DZb3tK5emJzmlM9tAxrHu2TJvhHIv2v5qs9lDgj3DhuXCZy4DvfSp19Zrxf0lJjAsu82t3L4mJq9qNDOrNDPtt0ZwgfQwhDbmMO3mwfPmZyWqxaZmdDWmJL1DwnxC2eWBdzJvJzJoweYl2XfzvzOAgr5q1HxB1Dnmg5jmJzfEueXvKS2D3zkv25iza#AqS4mZm");
    QTest::newRow("Relanium vs. Linkin Park - Numb Wait (Relanium &amp; Alex Sprinter Bootleg)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=lOqWodH0ChrkDffJCgHJzf01uYO3CxHtEvi4mJHiDxnKCtrOvevRvxrFAxi9Dc5VzKHWmZf2CY9LDI5kthjUDM5zDtqVtgjkmLb1tKXYl2T4B2e2nwuOtwTZBee3uJzVpOvXutjXrNeOywWVmgfVlJvjntu1mhv0DKLosKzUrviZoJa1tgTlDefZzLK1vvyXmfq2m2nFzxiXDgvzrZzFyOn0vJLiqOfUoxmZlsOOmhDJtMPrC3HhvO1VmOfZAtnqBuHcqw1lwxjpvdfwlu5AnJLXCu9SnZCVDhHSDgf1t3zKnuq#AqS2mZq");
    QTest::newRow("Rico Bernasconi &amp; Tuklan ft. A-Class &amp; Sean Paul - Ebony Eyes") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=BZD2qMeVoJCVDJbOlLbMsLHbovnOAxeWyunkDNzjrJbWts9Osg9WAO4OC3vhwuiTve5ZnMHiuMjPtw5FC1ngnM5KmdrLngrpmfzkq2S3AtrJmLfetdnHCZvJzJnHCKrVzfaXDOznDwvLDuzJotnitfLhDhvZlY4YEvDLAg1RmNvjnNDLDwrtrtLQBuW3CMPTDhH4nvyOrgv2weXKA3flnvbwC1HQsMy2nuL2Cc5YpY1Wlxnwsvi5C3HuvhnZtdDImta1vvzWlum2l3bnCJj6neLSyZu5ru5ZmMzrDNu9qwfhl3uX#AqSXndu");
    QTest::newRow("David Argunetta feat. Sarkis Edwards - So Nice (Dj Andy Light &amp; Dj O&#39;Neill Sax Remix) | (Диджей играющий на саксофоне +7(991)999-97-99 СПБ)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=vMfhwKHdAKiTqunAt2vYC1foAwS5BwDpwxmZmNzWlLKYEMfPmumVytL5CwroC3aYr3r2Aw95lZGYEgmVmdb1mvCYsw5Qv25hmv01zxu2ttf4EfPWyuXfv3r2yMjjoc81lNrKCMLZDhrzBJyYAJmWwvfJthHKAhbvnxrVBwrOrJfbut9ADJnNC2HkyLPHDdnTEvPlthzTv1vRtgK5B1G4ueD4sfi6DtzxmZm5pwmOsM5Lv3jTBML5yuLYxOHQlLK5vdHdBc9wA29Nl3jlvY1smZzLBwrqm3rInZrNmgv2rOL1#AqSYmdu");
    QTest::newRow("The Pharcyde - Oh Shit") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Ahm5tgvHn2fQvwX1Bw15ohPHDNPunMPyAtrqENbKEg5YnxbomxuZDLHXoevHvML1wK9cltf2vvjOmZnOCgDFD21KlY51Cc9fztPMyuHJmZaWAI1rBZjfEeuWCNbXqOXoyI9FC2vsCd12B3HKAwvevu82EdnxDtbSlY4Xy19pquvmB1PFDhn4vteVEg9puJzKnhrTn1PmDxn6lJbHogLKz1DcoxDQCMG1t1LVlO84ChnlzJj0qOzfrMrsrtffCgqXy1DQwL04A214BgDSwxC1wJrpzJvWvJv1ENyZBd91rZy#AqSOnda");
    QTest::newRow("EMF - You&#39;re Unbelievable") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdnRAu5Hyt12EJneuL9yshnIqvj2DvvODeHHtNrOC1DNqvD5nKvgCfDeA1PHBhbIwI4VtwT2mu1WtMTjlZvdyOX2ytrWyxfex2rQter2uNvQoNv2og4OtZjUExjJnuvLtg1LCfbpC304t3KXnePXA29RDvnIAwfpChGXDKXZzgu4m1HUn30Ymu1TA2KYCI81tKreudHZlNDYBLjJrgLRyMe5DxeYwePMz25ls3KXAeuVCOmUoujmutvfvJbnBMnlp2iTztrntNrLCZLyCdrAzcOTsvu#AqS4mdy");
    QTest::newRow("Stan Bush - The Touch (OST Saints Row 4)") << QByteArray("");
    QTest::newRow("Alexandra Stan feat. Carlprit - 1.000.000 (Rico Bernasconi Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=lOC4Dt8ZmZHOms41su1OnxDTB2zYnLLdrMPry3nVmtH0mfuOugzqnwT2sgvMAwv6yKO2mg13z2uTlwnnne1JDhjZAdPAy3vLm3yZyvrsCKGYBw1Wl24UmZnPCdvNsJjXqOjKs2f6A25UCtjTy2TUmgDluKK3Ehy4nhi1rhPFlNPLBhK3yuz1zOz0zfHvvMOOsdfnBgfZutnVBue9oePiyOfutZrvvuKXu2rJq3rHuuyYAOu5shiVCxrLyxzLBJbiExzpnwO3C2fOBwr0BxbjlO4YyG#AqSYnJy");
    QTest::newRow("BBX - Making Luv (Extended Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=Agv6wKSOrZCVpvDHtxD2y3npu3vtAMrTmNvsl2j4AwSUELGXwhbJBs4XoeWVAhuXywT1qtm3oMPJmJjlCg9HC1HFzgXvDwSOl2yXt2rJmxrcztrom21xt2OYztzoBuq/DJjowLGOmZm5wenSr29yyxrOlLHKCuLHy2vWnNbIAxbYzgr3qZj1DMnLCenZEfnRugvQDND5DOrZEdf4Bv9HmKLpnhrNCNvKn2XUuMKYAujonNLOs1nvmLbcmtnemxrUmdnRCdiVmLG5r3bYrgHHDc1Nl3PxrZD4DvbLstK5zZu#AqS2odC");
    QTest::newRow("Enrique Iglesias  - Bailando (Dj Tarantino Club Radio Mix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdztEgTwyNjUmvLLm2rWsLnLrvPeENnhu2v1EgLIrM1ZzMPUru8VCZGTmI5tl2L6DdzFuc52l2rWl3i6B29NwLqTyvyWz1z2AMeXDMSYAgvfyxDJs3HvmZzAow5umxb2v1b1vJ9HuLbOlv9fDtvuutDdme9ZCdfUnJbJDY5WnZDkDtfsAgfLAZzHmJiXnOzuAdf1mwLLuvrSzdm4vgXemhG5DhfAA1zpovvSmw9Kls1JlxzJzuzRmxm9thm2A2i2ChjxwLzLuMOWDtGZDgfendrcne8#AqSOodG");
    QTest::newRow("Ed Sheeran - Photograph (Felix Jaehn Remix)") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdH1ut8YA1zUqv9WlLyYsZrXqs13Agv4mxq5EMrnmwrZwhrPB3n5C2OOlxfYnc9Ly1KYsvyVuxbol1Pizvv2m2LzyK9OrfGYoM83ou8YBODKytvOmL9fDfHduLrTBuiWudn6tNq1DgeZzZrYy3KWuZzYDKf1ue1UmwuVlL9yqJv0zfLwofaXzfz1rJvSDdDZr2ziBhfzDhLTAg45ANjLztOXBu9nBwiYmdDuzc5LytvnAfHVDxjLwue3AOeOvZmTvO12CfHsxOLPx2DVwvj4BfD4CgW#AqS3odC");
    QTest::newRow("Adam Lambert - Running") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=AdrJD1PbvwHysJrRCO53DevLrKPIyOjewJj3AdztltHOtxrXBMXAyZKUneOVALKXwvbLmY5WqMPwrK5FCNHgtgzSmJ1ymtHTAxbrvdDIyOfrC1nLvfnVoJvOsO9blI1oAffJDvaZCw9dthu4De1Nohvzy3b2zxHsCgzzDJCVzwuTv2rtl2vPrtrzndzszLDdBhLKvfLbAwDToe93q3zPqwm2xO04zZbHnZDKx3rHuKDjzKmOohjbmhzLnOXbDOCTp3DcAfPIC2e4Cc1cnhbFl2jHCa#AqS1mdi");
    QTest::newRow("The Cab - Angel With A Shotgun") << QByteArray("https://vk.com/mp3/audio_api_unavailable.mp3?extra=sxKXCNvHAY1zzwC4CI8ODtOTlMvmlZaVoeTKAZfPAI1SAZC1mZjem3rvlNr0CZfVDxvKuJzmnY83nvvZBteYDvPSrg1Uow1tCJDJCwngzNvSC3vetMDrALv5vZq3z3zWsgS6ugXOnhqYD3GXz1bFqNb0rMvHEtHWCxronxqOD25YD1rHmdL4BgTxvtGTmd9YvM9PzxnXsMq3mJj1yZfSmJKXBgyWA2TNlNbqvgHuoenLouHcCKS3tfvNn2vWmvrxBhvHC2HrCwu3zhz2mwXpy3vYvZG#AqS2nZm");
}

void TestVK::decodeUrl()
{
    QFETCH(QByteArray, encoded);
    const auto res = VKResponse::decodeAudioUrl(88986886, encoded);
    QEXPECT_FAIL("Marc Korn &amp; Clubraiders feat. Orry Jackson - Everybody Likes To Party (Radio Mix)", "Link unavailable", Continue);
    QEXPECT_FAIL("Stan Bush - The Touch (OST Saints Row 4)", "Link unavailable", Continue);
    QVERIFY(QUrl(res).isValid());
}

void TestVK::testPlaylist()
{
    auto model = std::make_unique<VKItemModel>();
    model->setFetchLimit(1000);
    model->insertRows(0, 1000);

    auto playlist = std::make_unique<Playlist>(model.get());

    playlist->setCurrent(9);
    QVERIFY(playlist->previous() == 8);
    QVERIFY(playlist->current() == 9);
    QVERIFY(playlist->next() == 10);

    model->removeRows(10, 10);
    QVERIFY(playlist->previous() == 8);
    QVERIFY(playlist->current() == 9);
    QVERIFY(playlist->next() == 10);

    model->removeRows(0, 9);
    QVERIFY(playlist->previous() == -1);
    QVERIFY(playlist->current() == 0);
    QVERIFY(playlist->next() == 1);

    model->insertRows(0, 100);
    QVERIFY(playlist->previous() == 99);
    QVERIFY(playlist->current() == 100);
    QVERIFY(playlist->next() == 101);

    model->removeRows(0, 101);
    QVERIFY(playlist->previous() == -1);
    QVERIFY(playlist->current() == -1);
    QVERIFY(playlist->next() == -1);
}

void TestVK::cleanupTestCase()
{
    delete m_vk;
}

QTEST_GUILESS_MAIN(TestVK)
#include "testvk.moc"
