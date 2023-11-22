# Spínání světla dle intenzity #

Vedoucí: [Ing. Vojtěch Mrázek, Ph.D.](https://www.fit.vut.cz/person/mrazek/.en)

## Zadání ##

Cílem je měřit úroveň svitu a podle toho nastavovat úroveň svitu diody. Aplikace by měla být uživatelsky nastavitelná (např. přes sériovou komunikaci), hodnota by se měla posílat na internet přes MQTT.

Desku ESP32 s periferiemi si vyzvednete u vedoucího (před tím vám pošle e-mail).

## Požadavky ##

1. K programování můžete využít prostředí Arduino nebo IDF. Na hodnocení se bude promítat i komplexnost řešení – pokud použijete Arduino, očekává se, že pro získání plného počtu bodů se budete snažit aplikaci co nejvíce "dotáhnout".
2. Data ukládejte do vnitřní paměti, využijte NVS modulu.
3. Při ovládání LED diody pamatujte na to, že úroveň svitu by měla být linearizovaná.
4. Umožněte uživatelům nastavit prahovou teplotu, při které se provede akce (např. sepnutí LED diody) - simulace ovládání. Při spínání pracujte s tzv. hysterezí.
5. Nastavení můžete provádět přes sériovou komunikaci, nastavení by mělo zůstat v přístroji i po odpojení napájení.
6. Aktuální hodnotu svitu posílejte pravidelně do MQTT brokeru, v externí aplikaci ji můžete sledovat.
7. Vycházejte z [dokumentace IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/index.html), popisu v [e-learningu](https://moodle.vut.cz/course/view.php?id=268243) a [ukázek](https://github.com/espressif/esp-idf/tree/master/examples/).
8.
    - Vytvořte přehlednou dokumentaci k přípravě, způsobu realizace, k funkčnosti a vlastnostem řešení projektu.
    - Řešení (projekt, bez binárních souborů sestavitelných na základě zdrojových souborů v projektu, a dokumentaci ve zdrojové i binární, tj. PDF, podobě) odevzdávejte prostřednictvím IS v jediném ZIP archívu.
    - Předvedení řešení se předpokládá buď v osobní podobě, s následným vrácením zapůjčeného vybavení, nebo s odevzdáním videa (např. na youtube, google drive a podobně). V případě nejasností však můžete být vyzváni k osobní prezentaci řešení. Zařízení potom navrátíte vedoucímu
    - Hodnocení projektu bude provedeno na základě: dokumentace (4 body), funkčnosti (5), prezentace (1) a kvality řešení (4). Samozřejmě u nefunkčního řešení nemůže být plné hodnocení dokumentace, protože zjevně nepopisuje pravý stav věci. Podobně v kvalitě hodnocení bude zahrnuto i to, jestli vaše řešení splňuje minimální požadavky a navíc se volají jen 3 funkce z knihovny, či je dodělané z pohledu řešení různých chyb, domyšlení ovládání či případného rozšíření.
