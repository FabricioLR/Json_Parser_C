#include "../json.h"


int main(int argc, char **argv){
    char *json_string = "{" \
                            "\"Image\":{" \
                                "\"Thumbnail\":{" \
                                    "\"Url\":\"http:/*www.example.com/image/481989943\"," \
                                    "\"Height\":125," \
                                    "\"Width\":\"100\"," \
                                    "\"test\":{" \
                                        "0:\"hello\"" \
                                    "}" \   
                                "}," \
                                "\"Width\":800," \
                                "\"Height\":600," \
                                "\"Title\":\"Viewfrom15thFloor\"," \
                                "\"IDs\":[116,943,234,{\"first\":38793},555]" \
                            "}," \
                            "\"Video\":{" \
                                "\"Width\":400," \
                                "\"Height\":300" \
                            "}" \
                        "}";

	JSON *json = json_parse(json_string);

    print_json(json);

	JSON *image = json_get_item(json, "Image");

    JSON *width = json_get_item(image, "Width");

    printf("%s\n", json_get_value(width));

	return 0;
}