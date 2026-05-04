//
// Created by rudri on 10/11/2020.
//

#ifndef PROG3_UNIT1_TUIT_V2023_1_TUIT_H
#define PROG3_UNIT1_TUIT_V2023_1_TUIT_H
#include <string>
#include<vector>
void question_1();
class Element {
    int id;
    public:
    explicit Element(int id_):id(id_){}
    virtual std::ostream& show(std::ostream& os)=0;
    virtual ~Element()=default;
};
class Media:public Element {
    std::string file_name;
    public:
    explicit Media(int id_,std::string file_name_):Element(id_),file_name(std::move(file_name_)){}
    std::ostream& show(std::ostream& os)override {
        os<<file_name<<"\n";
        return os;
    };
};
class Emoji:public Element {
    std::string simbol;
    public:
    explicit Emoji(int id_, std::string simbol_):Element(id_),simbol(std::move(simbol_)){}
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
        return os<<title<<"\n";
    };
};
class Tuit{
    std::string user_name;
    std::string text;
    std::vector<Element*> elements;
    std::vector<Tuit> replies;
    public:
    Tuit(std::string user_name_,std::string text_):user_name(std::move(user_name_)),text(std::move(text_)){};
    void add_element(Element* element) {
        elements.push_back(element);
    };
    void add_reply(Tuit& tuit) {
        replies.push_back(tuit);
    }
    Tuit& show(std::ostream& os) {
        return *this;
    }

    void save_to_file(std::string file_name) {

    }
};
#endif //PROG3_UNIT1_TUIT_V2023_1_TUIT_H
