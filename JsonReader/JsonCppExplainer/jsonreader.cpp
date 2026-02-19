#include <fstream>
#include <iostream>
#include "json.h"

int main(){

    // Читання .json з файлу
    Json::Value root;
    // Json::Value внутрішня струтура класу:
    /* class Value{
        public:
            <Методи>
        private:
            union ValueHolder{
                int int_;
                long long_;
                double real_;
                char* string_;
                ObjectValues* map_; - std::map
                ArrayValues* array_; - std::vector
            } value_;

            ValueType type_ : 8; - для зберігання типу, який містить цей об'єкт
    }
    */
    // Принцип роботи:
    // 1. Зчитує .json файл/string
    // 2. Якщо файл починається { - то виділяє пам'ять для map, [ - для vector
    // 3. map<Value/vector, string> - тобто, якщо далі іде змінна вона також об'єкт Value - тільки виділяє пам'ять під змінну 

    // В цій бібліотеці є головні касти класів:
    // 1. FastWriter, Reader - застарілі класи, які просто читають або записують все одним рядком
    // 2. CharReader -> CharReaderBuilder, StreamWriter -> StreamWriterBuilder    -     це класи, які не можуть існувати одне без одного, адже
    //                                                                                  класи типу "Reader, Writer" - займаються ось цією головною роботою
    //                                                                                  , а Builder налаштовують їх - чи читати коментарі, чи форматувати текст(додавати \n)

    std::ifstream ifs("jsonexample.json");

    Json::CharReaderBuilder builder;

    // String - у який записується помилка, якщо файл не вдалось прочитати
    JSONCPP_STRING errs;
    if(!Json::parseFromStream(builder, ifs, &root, &errs)){
        std::cout << errs << std::endl;
        return 1;
    }

    std::cout << root << std::endl;








    // Читання .json з string
    const std::string rawJson(R"({"name" : "Ivan", coursesDone : [1, 2]})");
    const int rawJsonLength = rawJson.length();

    bool shouldUseOldWay = false;
    
    JSONCPP_STRING errs1;
    Json::Value root1;

    if(shouldUseOldWay){
        // Об'єкт, який читає .json string
        Json::Reader reader;
        reader.parse(rawJson, root1);
    }
    else{
        // Більш сучасний спосіб читання .json string
        // Саме тут показується - CharReader залежить від CharReaderBuilder
        const std::unique_ptr<Json::CharReader> reader(Json::CharReaderBuilder().newCharReader());
        if(!reader->parse(rawJson.c_str(), rawJson.c_str() + rawJsonLength, &root1, &errs1)){
            std::cout << "error: " << errs1 << std::endl;
            return 1;
        }
    }

    // Перезавантажений оператор індексації повертає клас Json::Value
    const std::string name = root["name"].asString();
    auto temp = root["coursesDone"];
    std::vector<int> vec;

    for(int i = 0; i < temp.size(); ++i){
        vec.push_back(temp[i].asInt());
        std::cout << i << " :" << temp[i].asInt() << std::endl;
    }

    std::cout << name << std::endl;










    // Запис .json у файл(потік)
    Json::Value root2;
    Json::StreamWriterBuilder builder1;
    const std::unique_ptr<Json::StreamWriter> writer(builder1.newStreamWriter());

    // Створює std::map і додає туди таке значення
    root2["Name"] = "Ivan";
    root2["Age"]  = 19;

    // Запис у потік
    writer->write(root2, &std::cout);








    // Запис .json у string
    Json::Value root3;
    Json::Value data;

    bool shouldUseOldWay1 = false;

    root3["action"] = "run";
    data["number"] = 1;
    root3["data"] = data;

    if(shouldUseOldWay1){
        Json::FastWriter writer;
        const std::string json_str = writer.write(root3);
        std::cout << json_str << std::endl;
    }
    else{
        Json::StreamWriterBuilder builder3;
        const std::string json_str = Json::writeString(builder3, root3);
        std::cout << json_str << std::endl;
    }
 
    return 0;
}