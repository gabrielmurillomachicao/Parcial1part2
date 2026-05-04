//
// Created by rudri on 10/11/2020.
//

#ifndef PROG3_UNIT1_TUIT_V2023_1_TUIT_H
#define PROG3_UNIT1_TUIT_V2023_1_TUIT_H
#include <string>
#include<vector>
#include<fstream>
class Element {
    int id;
    public:
    explicit Element(int id_):id(id_){}
    virtual std::ostream& show(std::ostream& os)=0;
    virtual std::ostream& save_to(std::ostream& os)=0;
    virtual ~Element()=default;
    [[nodiscard]]int  get_id() const {
        return id;
    };

};
class Media:public Element {
    std::string file_name;
    public:
    explicit Media(int id_,std::string file_name_):Element(id_),file_name(std::move(file_name_)){}
    std::ostream& save_to(std::ostream& os) override {
        os<<"Emoji|"<<get_id()<<"|"<<file_name<<std::endl;
        return os;
    };
    std::ostream& show(std::ostream& os)override {
        os<<file_name<<std::endl;
        return os;
    };
};
class Emoji:public Element {
    int size;
    std::string simbol;
    public:
    Emoji(int id_, std::string simbol_, int size_):Element(id_),simbol(std::move(simbol_)),size(size_){}
    std::ostream& save_to(std::ostream& os) override {
        os<<"Emoji|"<<get_id()<<"|"<<simbol<<"|"<<size<<std::endl;
        return os;
    }
    std::ostream& show(std::ostream& os)override {
        os<<simbol<<std::endl;
        return os;
    }
};
class Poll:public Element {
    std::string title;
    std::vector<std::string> choices;
    int days;
    int hours;
    int minutes;
    public:
    explicit Poll(int id_, std::string title_,std::vector<std::string> choices_,int days_,int hours_,int minutes_):Element(id_),title(std::move(title_)),choices(std::move(choices_)),days(days_),hours(hours_),minutes(minutes_){}
    std::ostream& show(std::ostream& os)override {
        os<<title<<std::endl;
        for (std::string choice : choices) {
            os<<"- "<<choice<<std::endl;
        }
        return os;
    };
    std::ostream& save_to(std::ostream& os) override {
        os<<"Poll|"<<title<<"|";
        for (int i=0;i<choices.size();i++) {
            if (i!=choices.size()-1) {
                os<<choices[i]<<",";
            }
            else {
                os<<choices[i];
            }
        }
        os<<"|"<<days<<"|";
        os<<hours<<"|";
        os<<minutes<<std::endl;
        return os;
    }
};
class Tuit {
    std::string user_name;
    std::string text;
    std::vector<Element*> elements;
    std::vector<Tuit> replies;
public:
    Tuit()=default;
    Tuit(std::string user_name_,std::string text_):user_name(std::move(user_name_)),text(std::move(text_)){};
    Tuit& add_element(Element* element) {
        elements.push_back(element);
        return *this;
    };
    Tuit& add_reply(const Tuit& tuit) {
        replies.push_back(tuit);
        return *this;
    }
    Tuit& show(std::ostream& os) {
        os<<"===================="<<std::endl;
        os<<"@"<<user_name<<std::endl;
        os<<text<<std::endl;
        os<<"--------------------"<<std::endl;
        for (auto element : elements) {
            element->show(os);
        }
        for (auto element : replies) {
            element.show(os);
        }
        return *this;
    }
    Tuit& load_from(const std::string& file_name) {
        std::ifstream file(file_name);
        std::string line;
        int reply=0;
        while (std::getline(file, line)) {
            int line_size = line.size();
            std::string tipo;
            bool signal =true;
            int pos=0;
            int inicio=0;
            int numero=0;
            int id;
            int universal_int;
            int universal_int2;
            int universal_int3;
            std::string universal_string1;
            std::string universal_string2;
            std::string universal_string3;
            std::vector<std::string> choices;
            while (signal) {
                std::string word=line.substr(pos,1);
                if (word=="|"&&numero==0) {
                    tipo=line.substr(inicio,pos);
                    inicio=pos+1;
                    numero++;
                }
                else if (reply==0||reply==1) {
                    if (word=="|"&&tipo=="Tuit"&&numero==1) {
                        std::string key=line.substr(inicio,pos-inicio);
                        user_name=key;
                        inicio=pos+1;
                        numero++;
                    }
                    //Probado
                    else if (pos==line_size&&numero==2&&tipo=="Tuit") {
                        std::string key=line.substr(inicio,pos-inicio);
                        text=key;
                        inicio=pos+1;
                        numero++;
                        reply+=1;
                        signal=false;
                    }
                    //-------------Probado Tuit----------------//
                    //Termino Tuit
                    //-----------------Empiezan los elementos----------------//
                    //Empieza Emoji
                    else if (word=="|"&&numero==1&&tipo=="Emoji") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Emoji"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==3&&tipo=="Emoji"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_int2=stoi(key);
                        add_element(new Emoji(id,universal_string1,universal_int2));
                        signal=false;

                    }
                    //--------------Ya esta probado Emoji------------//
                    //----------------Media--------------------//
                    else if (numero==1&&tipo=="Media"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Media"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        add_element(new Media(id,universal_string1));
                        reply++;
                        signal=false;
                    }
                    //-------------Probado Media-----------------//
                    }
                else {
                    if (word=="|"&&tipo=="Tuit"&&numero==1) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        inicio=pos+1;
                        numero++;
                    }
                    else if (pos==line_size&&numero==2&&tipo=="Tuit") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string2=key;
                        inicio=pos+1;
                        numero++;
                        add_reply(Tuit(universal_string1,universal_string2));
                        reply+=1;
                        signal=false;
                    }
                    //Termino Tuit
                    //-----------------Empizan los elementos----------------//
                    //Empieza Emoji
                    else if (word=="|"&&numero==1&&tipo=="Emoji") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Emoji"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==3&&tipo=="Emoji"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_int2=stoi(key);
                        replies[reply-3].add_element(new Emoji(id,universal_string1,universal_int2));
                        signal=false;

                    }
                    //----------------Media--------------------//
                    else if (numero==1&&tipo=="Media"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Media"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        replies[reply-3].add_element(new Media(id,universal_string1));
                        signal=false;
                    }
                    //------------Poll--------------//
                    else if (numero==1&&tipo=="Poll"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Poll"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==3&&tipo=="Poll") {
                        if (word=="|") {
                            std::string key=line.substr(inicio,pos-inicio);
                            choices.push_back(key);
                            inicio=pos+1;
                            numero++;
                            }
                        else if (word==",") {
                            std::string key=line.substr(inicio,pos-inicio);
                            choices.push_back(key);
                            inicio=pos+1;
                        }
                    }
                    else if (numero==4&&tipo=="Poll"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);

                        universal_int=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==5&&tipo=="Poll"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_int2=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==6&&tipo=="Poll"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_int3=stoi(key);
                        replies[reply-3].add_element(new Poll(id,universal_string1,choices,universal_int,universal_int2,universal_int3));
                        signal=false;
                    }
                }
                pos++;
            }

        }
        return *this;
    }
    Tuit& load_from(std::istream& is) {
        std::string line;
        int reply=0;
        while (std::getline(is, line)) {
            int line_size = line.size();
            std::string tipo;
            bool signal =true;
            int pos=0;
            int inicio=0;
            int numero=0;
            int id;
            int universal_int;
            int universal_int2;
            int universal_int3;
            std::string universal_string1;
            std::string universal_string2;
            std::string universal_string3;
            std::vector<std::string> choices;
            while (signal) {
                std::string word=line.substr(pos,1);
                if (word=="|"&&numero==0) {
                    tipo=line.substr(inicio,pos);
                    inicio=pos+1;
                    numero++;
                }
                else if (reply==0||reply==1) {
                    if (word=="|"&&tipo=="Tuit"&&numero==1) {
                        std::string key=line.substr(inicio,pos-inicio);
                        user_name=key;
                        inicio=pos+1;
                        numero++;
                    }
                    //Probado
                    else if (pos==line_size&&numero==2&&tipo=="Tuit") {
                        std::string key=line.substr(inicio,pos-inicio);
                        text=key;
                        inicio=pos+1;
                        numero++;
                        reply+=1;
                        signal=false;
                    }
                    //-------------Probado Tuit----------------//
                    //Termino Tuit
                    //-----------------Empiezan los elementos----------------//
                    //Empieza Emoji
                    else if (word=="|"&&numero==1&&tipo=="Emoji") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Emoji"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==3&&tipo=="Emoji"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_int2=stoi(key);
                        add_element(new Emoji(id,universal_string1,universal_int2));
                        signal=false;

                    }
                    //--------------Ya esta probado Emoji------------//
                    //----------------Media--------------------//
                    else if (numero==1&&tipo=="Media"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Media"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        add_element(new Media(id,universal_string1));
                        reply++;
                        signal=false;
                    }
                    //-------------Probado Media-----------------//
                    }
                else {
                    if (word=="|"&&tipo=="Tuit"&&numero==1) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        inicio=pos+1;
                        numero++;
                    }
                    else if (pos==line_size&&numero==2&&tipo=="Tuit") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string2=key;
                        inicio=pos+1;
                        numero++;
                        add_reply(Tuit(universal_string1,universal_string2));
                        reply+=1;
                        signal=false;
                    }
                    //Termino Tuit
                    //-----------------Empizan los elementos----------------//
                    //Empieza Emoji
                    else if (word=="|"&&numero==1&&tipo=="Emoji") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Emoji"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==3&&tipo=="Emoji"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_int2=stoi(key);
                        replies[reply-3].add_element(new Emoji(id,universal_string1,universal_int2));
                        signal=false;

                    }
                    //----------------Media--------------------//
                    else if (numero==1&&tipo=="Media"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Media"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        replies[reply-3].add_element(new Media(id,universal_string1));
                        signal=false;
                    }
                    //------------Poll--------------//
                    else if (numero==1&&tipo=="Poll"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        id=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==2&&tipo=="Poll"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_string1=key;
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==3&&tipo=="Poll") {
                        if (word=="|") {
                            std::string key=line.substr(inicio,pos-inicio);
                            choices.push_back(key);
                            inicio=pos+1;
                            numero++;
                            }
                        else if (word==",") {
                            std::string key=line.substr(inicio,pos-inicio);
                            choices.push_back(key);
                            inicio=pos+1;
                        }
                    }
                    else if (numero==4&&tipo=="Poll"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);

                        universal_int=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==5&&tipo=="Poll"&&word=="|") {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_int2=stoi(key);
                        inicio=pos+1;
                        numero++;
                    }
                    else if (numero==6&&tipo=="Poll"&&pos==line_size) {
                        std::string key=line.substr(inicio,pos-inicio);
                        universal_int3=stoi(key);
                        replies[reply-3].add_element(new Poll(id,universal_string1,choices,universal_int,universal_int2,universal_int3));
                        signal=false;
                    }
                }
                pos++;
            }

        }
        return *this;
    }
    Tuit& save_to(std::string file_name) {
        std::ofstream fout;
        fout.open(file_name);
        fout<<"Tuit"<<"|"<<user_name<<"|"<<text<<std::endl;
        for (auto element:elements) {
            element->save_to(fout);
        }
        for (auto element:replies) {
            element.save_to(fout);
        }
        return *this;
    }
    std::ostream& save_to(std::ostream& os) {
        os<<"Tuit"<<"|"<<user_name<<"|"<<text<<std::endl;
        for (auto element:elements) {
            element->save_to(os);
        }
        for (auto element:replies) {
            element.save_to(os);
        }
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Tuit& t);
    friend std::ostream& operator<<(std::ostream& os,  Tuit& t);
};
inline std::istream& operator>>(std::istream& is, Tuit& t) {
    t.load_from(is);
    return is;
}
inline std::ostream& operator<<(std::ostream& os,  Tuit& t) {
    t.save_to(os);
    return os;
}
#endif //PROG3_UNIT1_TUIT_V2023_1_TUIT_H
