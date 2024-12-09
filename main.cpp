//Jere Lehtimäki
//Huonevaraus

#include "includes_and_prototypes_h_file.h"
#include "struct_h_file.h"

using namespace std;

//HUOM. Koodissa käytetty "line" kun luetaan koko rivi tiedostosta, ja "word" kun luetaan yksi sana aijemmin mainitulta riviltä!

// Tarkistaa onko str muodossa tullut syöte numero ja annetulla välillä. Mikäli ei, pyytää uuden syötteen kunnes numero annetulta väliltä on saatu ja palauttaa sen jälkeen integerin.
// anyof ja lambdafunktioiden käyttö opittu netistä stackoverflown ja muiden dokumentaatioiden kautta.
int numbercheckfunction(string num, int alaraja, int ylaraja) {
	while (any_of(num.begin(), num.end(), [](char i) { return !isdigit(i);  }) || stoi(num) < alaraja || stoi(num) > ylaraja) {
		cout << "Olkaa hyvät ja antakaa numero väliltä " << alaraja << " ja " << ylaraja << "." << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> num;
	}
	return stoi(num);
}

bool textcheck(char i) {
	return isdigit(i) || i == '„' || i == '”';
}

//tarkistaa ettei nimi sisällä numeroita tai ääkkösiä. Koodiin saatu apua stackoverflowista.
string namecheckfunction(string word, int max_length) {
	while (any_of(word.begin(), word.end(), [](char i) { return textcheck(i); }) || word.size() > max_length || word.size() < 1) {
		cout << "Virheellinen nimi. Huomaathan että nimessä ei voi olla esimerkiksi numeroita, ääkkösiä tai merkkejä.\n";
		if (word.size() > max_length)
			cout << "anna maksimissaan " << max_length << " mittainen nimi.\n";
		getline(cin, word);
	}
	return word;
}

//tarkistaa ettei nimi sisällä numeroita tai ääkkösiä. Koodiin saatu apua stackoverflowista.
string textcheckfunction(string word, int max_length) {
	while (any_of(word.begin(), word.end(), [](char i) { return textcheck(i); }) || word.size() > max_length || word.size() < 1) {
		cout << "Virheellinen sana. Huomaathan että sanassa ei voi olla esimerkiksi numeroita, ääkkösiä tai merkkejä.\n";
		if (word.size() > max_length)
			cout << "anna maksimissaan " << max_length << " mittainen sana.\n";
		getline(cin, word);
	}
	return word;
}

//Avaa tekstitiedoston vektoriksi joka täytetään structeilla
//Eka rivi sisältää max-rooms määrän, eli kokonaishuonemäärän, ja loput rivit ovat varaus per rivi
void txt_to_struct(vector<room>& rooms ,int& max_rooms) {
	string line, word;

	//avataan file, otetaan ensimmäiseltä riviltä max_rooms talteen
	ifstream file("reservations.txt");
	getline(file, line);
	max_rooms = stoi(line);

	//puretaan txt file rivi riviltä struct-muotoon.
	while (getline(file, line)) {
		room r;
		istringstream stream(line);

		getline(stream, word, ',');
		r.roomnum = stoi(word);

		getline(stream, word, ',');
		r.reservationnum = stoi(word);

		getline(stream, word, ',');
		r.name = word;

		getline(stream, word, ',');
		r.roomsize = stoi(word);

		getline(stream, word, ',');
		r.discount = stoi(word);

		getline(stream, word, ',');
		r.timeofstay = stoi(word);
		rooms.push_back(r);
	}
}

//Muuttaa structin koko koodin lopuksi talteen txt-fileen. Ensimmäinen rivi on aina huoneiden maksimimäärä, jotta tieto on tallessa.
void struct_to_txt(const vector<room>& rooms, int max_rooms) {
	ofstream file("reservations.txt");
	if (!file.is_open()) {
		cout << "Virhe, ei voitu avata tiedostoa." << endl;
		return;
	}

	//Huoneiden maksimimäärä talteen ekalle riville
	file << max_rooms << endl;

	//iteroidaan koko vektorin läpi ja kirjataan omalle rivilleen jokainen varaus.
	for (const auto& r : rooms) {
		file << r.roomnum << ",";
		file << r.reservationnum << ",";
		file << r.name << ",";
		file << r.roomsize << ",";
		file << r.discount << ",";
		file << r.timeofstay << endl;
	}
}

//Arpoo huoneiden maksimimäärän jos on ensimmäinen käynnistys ilman tiedostoa
int num_of_rooms() {
	srand(time(0));
	int max_rooms;
	do {
		max_rooms = rand() % (300 - 41) + 40;
	} while (max_rooms % 2 == 1);
	return max_rooms;
}

//tarkistusohjelma alemman funktion numerolle, eli varausnumerolle
//Iteroi läpi jokaisen varausnumeron vektorissa, ja palauttaa false jos ei löydy
bool IsUsedNumber(const vector<room>& rooms, int resnum) {
	for (const auto& r : rooms) {
		if (r.reservationnum == resnum) {
			return true;
		}
		else {
			return false;
		}
	}
}

//Luo varausnumeron, ja tarkistaa ettei se ole jo olemassa kenelläkään muulla.
int reservationnum_generator(const vector<room>& rooms, int& reservationnum) {
	do {
		srand(time(0));
		reservationnum = rand() % (99999 - 10001) + 10000;
	} while (IsUsedNumber(rooms, reservationnum));
	return reservationnum;
}

//Tarkistaa onko huone varattu
bool IsOccupied(const vector<room>& rooms, int room) {
	//käy läpi jokaisen vektorissa olevan structin, opittu stackoverflow ja chatgpt avulla
	for (const auto& r : rooms) {
		if (r.roomnum == room) {
			return true;
		}
	}
	return false;
}

//tarkistaa mikä on seuraava vapaa huone kokoluokan mukaan, ja palauttaa sen mainfunktioon.
int freeroomfunc(const vector<room>& rooms, int size, int max_rooms) {
	int room;
	if (size == 1) {
		room = 1;
		while (IsOccupied(rooms, room)) {
			room++;
		}
		if (room > max_rooms / 2) {
			cout << "Ei vapaita huoneita! \n";
			return 0;
		}
		return room;
	}
	else if (size == 2 ) {
		room = max_rooms / 2;
		while (IsOccupied(rooms, room)) {
			room++;
		}
		if (room > max_rooms) {
			cout << "Ei vapaita huoneita jäljellä! \n";
			return 0;
		}
		return room;
	}
}

//Laskee hinnan euroissa huonekoon ja alennuksen perusteella.
int pricecalculator(int roomsize, int ale) {
	if (roomsize == 1) {
		return 100 * (1.0 - (ale / 10.0));
	}
	else if (roomsize == 2) {
		return 150 * (1.0 - (ale / 10.0));
	}
}

//Struct - printtausfunktio, tulostaa tietyn structin tiedot jos tiedot haetaan nimellä, niin salaa varausnumeron.
void printstruct(room r, string name_or_num) {
	cout << "Varauksen tiedot:" << endl;
	cout << "Huoneen numero: " << r.roomnum << endl;
	if (name_or_num == "num") {
		cout << "Varausnumero: " << r.reservationnum << endl;
	}
	else {
		cout << "Varausnumero: Näkyy vain varausnumerohaulla." << endl;
	}
	cout << "Varaajan nimi: " << r.name << endl;
	cout << "Huoneen koko: " << r.roomsize << ". henkilölle." << endl;
	if (r.discount != 0) {
		cout << "Alennuksenne: " << r.discount * 10 << "%" << endl;
	}
	cout << "Hinta: " << pricecalculator(r.roomsize, r.discount) << " euroa." << endl;
	cout << "Huoneen varausaika: " << r.timeofstay << " päivää." << endl;
}

// Tiedon haku- ja muokkausfunktiot !!!!!

//Funktio päättämään jatketaanko muokkausta
bool edit_or_not() {
	string inputword;
	cout << "Jatketaanko muokkausta? k tai e" << endl;
	cin >> inputword;
	if (textcheckfunction(inputword, 1) == "e") {
		return false;
	}
	else if (textcheckfunction(inputword, 1) == "k") {
		return true;
	}
}

void data_edit(vector<room>& rooms, room& r, int max_rooms) {
	string inputnum, inputword;
	bool dataedit = true;

	// Tiedonmuokkausfunktio
	do {
		int lasku = 0;
		cout << endl << "Valitse mitä tietoja haluat muokata?" << endl;
		cout << "1 Varauksella oleva nimi \n2 Huoneen koko \n3 Varausaika \n4 Poista varaus \n5 Ei mitään" << endl;
		cin >> inputnum;
		switch (numbercheckfunction(inputnum, 1, 5)) {
		//Nimenvaihto
		case 1:
			cout << "Anna uusi nimi varaukselle (Ei ääkkösiä): " << endl;
			getline(cin, inputword);
			r.name = namecheckfunction(inputword, 50);

			//Jatketaanko muokkausta vai ei
			if (!edit_or_not()) {
				dataedit = false;
			}
			break;
		//Huoneen koon vaihto, vaihtaa samalla huonenumeron 
		case 2:
			cout << "Tahdotteko varmasti vaihtaa huoneen kokoa? k tai e" << endl;
			cin >> inputword;
			if (textcheckfunction(inputword, 1) == "k") {
				if (r.roomsize == 1) {
					r.roomsize = 2;
					r.roomnum = freeroomfunc(rooms, 2, max_rooms);
				}
				else if (r.roomsize == 2) {
					r.roomsize = 1;
					r.roomnum = freeroomfunc(rooms, 1, max_rooms);
				}
			}
			else if (textcheckfunction(inputword, 1) == "e") {
				continue;
			}
			//Jatketaanko muokkausta vai ei
			if (!edit_or_not()) {
				dataedit = false;
			}
			break;
		//Muutetaan varausaikaa
		case 3:
			cout << "Nykyinen varausaikanne on: " << r.timeofstay << " päivää." << endl;
			cout << "Kirjoittakaa alle kuinka pitkään tahtoisitte olla sen sijaan (maksimiaika 14 päivää): " << endl;
			cin >> inputnum;
			r.timeofstay = numbercheckfunction(inputnum, 1, 14);

			//Jatketaanko muokkausta vai ei
			if (!edit_or_not()) {
				dataedit = false;
			}
			break;
		//Poista varaus
		case 4:
			for (auto& temproom : rooms) {
				if (temproom.roomnum == r.roomnum) {
					rooms.erase(rooms.begin() + lasku);
				}
				lasku++;
			}
			cout << "Varaus poistettu." << endl;
			return;
		//Lopetuskäsky
		case 5:
			dataedit = false;
			break;
		}
		system("cls");
		cout << endl << "Kaikki tiedot viimeisimmillä muokkauksilla: " << endl;
		printstruct(r, "num");
	} while (dataedit);
}

void datasearch_reservationnum(vector<room>& rooms, int searchnum, const int max_rooms) {
	string inputnum, inputword;
	bool stopsearch = true, dataedit = true, found = false;
	do {
		for (auto& r : rooms) {
			if (r.reservationnum == searchnum) {
				found = true;
				system("cls");
				cout << "Löysimme varausnumerolla seuraavat tiedot: " << endl;
				printstruct(r, "num");

				//Muokataanko tietoja?
				cout << endl << "Haluatko muokata tietoja? Valitse..." << endl;
				cout << "1 Muokataan tietoja \n2 Ei muokata tietoja" << endl;
				cin >> inputnum;
				switch (numbercheckfunction(inputnum, 1, 2)) {
				case 1:
					data_edit(rooms, r, max_rooms);
					break;
				default:
					break;
				}
			}
		}

		cout << endl;
		if (!found) {
			cout << "Varausnumerolla ei löytynyt tietoja." << endl;
		}
		cout << "Tahdotteko..." << endl;
		cout << "1 Hakea uudelleen varausnumerolla? \n2 Lopettaa hakemisen?" << endl;
		cin >> inputnum;
		switch (numbercheckfunction(inputnum, 1, 2)) {
		case 1:
			cout << "Anna uusi varausnumero..." << endl;
			cin >> inputnum;
			searchnum = numbercheckfunction(inputnum, 10000, 99999);
			break;
		case 2:
			stopsearch = false;
			break;
		}
		

	} while (stopsearch);
}

//Alifunktio hakemaan tietoja nimellä.
int datasearch_name(const vector<room>& rooms, string searchname) {
	int namecount = 0;
	room match;
	for (const auto& r : rooms) {
		if (r.name == searchname) {
			namecount++;
			match = r;
		}
	}
	if (namecount == 1) {
		cout << "Löysimme nimellänne seuraavan varauksen: " << endl;
		printstruct(match, "name");
		return 1;
	}
	else if (namecount > 1 || namecount < 1) {
		return 0;
	}
}



int main() {
	setlocale(LC_ALL, "FI_fi");
	//huoneiden maksimimäärä
	int max_rooms;

	//Varaus vs haku switch-komentoa varten
	bool reservation = false, search = false;

	//Vastaavat nimiään. line käytössä tiedostonlukemisessa
	string name, inputword, inputnum, line;
	vector<room> rooms_vec;


	//Jos eka käynnistys, niin luodaan uusi lista. Muuten max_rooms saadaan alla olevasta funktiosta txt_to_struct joka ottaa max_rooms tiedostosta.
	ifstream file("reservations.txt");
	getline(file, line);
	if (line.empty()) {
		max_rooms = num_of_rooms();
	}
	else {
		txt_to_struct(rooms_vec, max_rooms);
	}

	//Päätetään ollaanko varaamassa huonetta vai etsimässä olemassaolevia varauksia läpi
	cout << "Hei! Tervetuloa hotelli Hauskaan!" << endl;
	cout << "Oletteko varaamassa uutta huonetta, vai hakemassa tietoja olemassaolevasta varauksesta? Valitkaa..." << endl;
	cout << "1 Varaamassa huonetta. \n2 Hakemassa olemassaolevaa varausta. \n3 Ei sittenkään mitään." << endl;
	cin >> inputnum;
	switch (numbercheckfunction(inputnum, 1, 3)) {
	case 1:
		reservation = true;
		break;
	case 2:
		search = true;
		break;
	case 3:
		return 0;
	}

	// VARAUSFUNKTIO
	while (reservation) {
		system("cls");
		srand(time(0));
		int size, varausaika, varattuhuone, discount, reservationnum;
		string freeroom;

		//Luodaan struct ja aletaan lisäämään tietoja
		room ReservationStruct;

		//Alennusarvonta
		ReservationStruct.discount = rand() % 3;

		//Luodaan uniikki varausnumero
		reservationnum_generator(rooms_vec, ReservationStruct.reservationnum);

		//aloitetaan hakemalla huonekoko, jonka jälkeen alifunktiolla huonelle numero, joka lisätään structiin, ja täytetään tietoja jo structiin
		cout << "Minkä kokoisen huoneen tahtoisitte varata?" << endl;
		cout << "Tarjolla on yhden hengen huoneita 100€ / yö ja kahden hengen huoneita 150€ / yö." << endl;
		if (ReservationStruct.discount != 0) { //Onnittelee alennuskampanjasta jos arpaonni antaa alennuksen
			cout << "Onnittelut! Kohdallenne osui kamppanja-alennus joka on: " << ReservationStruct.discount * 10 << "%" << endl;
		}
		cout << "Vastaattehan tähän antamalla joko\n1 jos tahdotte yhdelle henkilölle\n2 jos tahdotte kahdelle henkilölle." << endl;

		//Tiedustellaan huonekoko, jonka mukaan kerrotaan hinta, jossa on huomioitu alennus %. 
		// Lopuksi lisätään structiin vapaa huone
		cin >> inputnum;
		ReservationStruct.roomsize = numbercheckfunction(inputnum, 1, 2);
		system("cls");
		switch (ReservationStruct.roomsize) {
		case 1:
			if (ReservationStruct.discount == 0) {
				cout << "Valitsitte 1 hengen huoneen, jonka hinnaksi tulee: 100 euroa per yö!" << endl;
			}
			else {
				cout << "Valitsitte 1 hengen huoneen, jonka hinnaksi alennuksellasi tulee: " << pricecalculator(ReservationStruct.roomsize, ReservationStruct.discount) << " euroa per yö!\n";
			}
			break;
		case 2:
			if (ReservationStruct.discount == 0) {
				cout << "Valitsitte 2 hengen huoneen, jonka hinnaksi tulee: 150 euroa per yö!" << endl;
			}
			else {
				cout << "Valitsitte 2 hengen huoneen, jonka hinnaksi alennuksellasi tulee: " << pricecalculator(ReservationStruct.roomsize, ReservationStruct.discount) << " euroa per yö!\n";
			}
			break;
		}
		ReservationStruct.roomnum = freeroomfunc(rooms_vec, ReservationStruct.roomsize, max_rooms);


		//Nimensaanti 
		cout << "\nSeuraavaksi tarvitsemme teidän nimenne." << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, inputword);
		ReservationStruct.name = namecheckfunction(inputword, 50);


		//Varauksen kesto
		cout << "\nLoistavaa! Seuraavaksi tahtoisimme tietää kuinka pitkään tahdotte olla? Kertavarauksen maksimipituus on 14 päivää." << endl;
		cin >> inputnum;
		ReservationStruct.timeofstay = numbercheckfunction(inputnum, 1, 14);

		//Struct vektoriin
		rooms_vec.push_back(ReservationStruct);

		//Tulostetaan tehty varaus
		system("cls");
		cout << "MUISTA OTTAA VARAUSNUMERO TALTEEN!" << endl;
		printstruct(ReservationStruct, "num");
		cout << endl;


		//Pyydetään tieto, halutaanko uusi varaus ja pidetään huoli että vastaus voi olla vain k tai e
		cout << "Tahdotko tehdä lisää varauksia? Huomioithan, että jokainen varaus luodaan uudella varausnumerolla. Valitse alla olevista:" << endl;
		cout << "1 Uusi varaus\n" << "2 Ei uutta varausta, suljetaan ohjelma." << endl;
		cin >> inputnum;
		int continueres = numbercheckfunction(inputnum, 1, 2);
		if (continueres == 1) {
			reservation = true;
		}
		else {
			reservation = false;
		}
	}

	// HAKUFUNKTIO 
	while (search) {
		system("cls");
		int name_or_num, found;
		string name, hakusana, searchfor;

		//Varauksien haku nimellä tai varausnumerolla
		cout << "Varaushaku. Huomioi, että varauksen muokkaus on mahdollista vain varausnumerolla." << endl;
		cout << "Haetaan varaustietoja...\n 1. Nimellä?\n 2. Varausnumerolla?" << endl;
		cin >> inputnum;
		name_or_num = numbercheckfunction(inputnum, 1, 2);

		//Switch - komento josta voidaan etsiä joko nimellä tai varausnumerolla. Vain varausnumerohaku antaa muokata tietoja.
		switch (name_or_num) {
		case 1:
			cout << "Anna nimesi.\n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, inputword);
			system("cls");
			searchfor = namecheckfunction(inputword, 50);
			found = datasearch_name(rooms_vec, searchfor);
			// jos löytyy vain yksi varaus annetulla nimellä
			if (found == 1) {
				cout << endl << "Jos tahdotte muokata tietojanne, joudutte antamaan varausnumeron turvallisuussyistä. Pyydän valitsemaan seuraavista: " << endl;
				cout << "\n1 Etsitään varausnumerolla ja muokataan. \n2 Lopetetaan etsintä. " << endl;
				cin >> inputnum;
				//Kysytään jos käyttäjä tahtoo muokata varausnumerolla tietoja
				if (numbercheckfunction(inputnum, 1, 2) == 1) {
					cout << "Antakaa varausnumeronne: " << endl;
					cin >> inputnum;

					//Varausnumeroalifunktioon syötetään struct-vektori ja numerotarkistuksen läpi menevänä numerona varausnumero
					datasearch_reservationnum(rooms_vec, numbercheckfunction(inputnum, 10000, 99999), max_rooms);
					break;
				}
				else if (numbercheckfunction(inputnum, 1, 2) == 2) {
					break;
				}
			}
			//Jos nimellä löytyy enemmän kuin yksi varaus, vaatii suoraan varausnumeron
			else if (found == 0) {
				system("cls");
				cout << "Löysimme monta varausta samalla nimellä, joten joudumme turvallisuussyistä pyytämään teitä hakemaan varausnumerolla uudelleen." << endl;
				cout << "Antaisitteko varausnumeronne: " << endl;
				cin >> inputnum;
				datasearch_reservationnum(rooms_vec, numbercheckfunction(inputnum, 10000, 99999), max_rooms);
				break;
			}
		//Varausnumerohaku, joka sallii saman tien myös muokkauksen
		case 2:
			cout << "Anna varausnumerosi.\n";
			cin >> inputnum;
			datasearch_reservationnum(rooms_vec, numbercheckfunction(inputnum, 10000, 99999), max_rooms);
			break;
		}
		search = false;
	}

	//Lopuksi koko struct-vektorin tallennus txt tiedostomuotoon ennen ohjelman sulkemista
	struct_to_txt(rooms_vec, max_rooms);

	return 0;
}