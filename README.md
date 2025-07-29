# JSON Parser

![GitHub repo size](https://img.shields.io/github/repo-size/FabricioLR/Json_Parser_C?style=for-the-badge)
![GitHub language count](https://img.shields.io/github/languages/count/FabricioLR/Json_Parser_C?style=for-the-badge)
![GitHub forks](https://img.shields.io/github/forks/FabricioLR/Json_Parser_C?style=for-the-badge)
![Bitbucket open issues](https://img.shields.io/bitbucket/issues/FabricioLR/Json_Parser_C?style=for-the-badge)
![Bitbucket open pull requests](https://img.shields.io/bitbucket/pr-raw/FabricioLR/Json_Parser_C?style=for-the-badge)

> Implementação de json parser escrito em C.

## 🚀 Instalando

Apenas copie o arquivo ![json.h](https://github.com/FabricioLR/Json_Parser_C/blob/master/json.h) para dentro do seu projeto

## ☕ Usando

```
#include "json.h"

int main(int argc, char **argv){
     char *json_string = "{" \
                            "\"Image\":{" \
                                "\"Width\":800," \
                                "\"Height\":600" \
                            "}"
                        "}";

     JSON *json = json_parse(json_string);

     print_json(json);

     JSON *image = json_get_item(json, "Image");

     JSON *width = json_get_item(image, "Width");
     JSON *heigth = json_get_item(image, "Width");

     printf("Width: %s, Height: %s\n", json_get_value(width), json_get_value(height));
     ...
}
```

Saída esperada

```
$cd examples && make exec
JSON: {
	Image (OBJECT)
        	Width (OBJECT)
			800 (PRIMITIVE)
        	Height (OBJECT)
           		 600 (PRIMITIVE)
}
Width: 800, Height: 600
```


## 📫 Contribuindo

Para contribuir, siga estas etapas:

1. Fork este repositório.
2. Crie um branch: `git checkout -b <nome_branch>`.
3. Faça suas alterações e confirme-as: `git commit -m '<mensagem_commit>'`
4. Envie para o branch original: `git push origin <nome_do_projeto> / <local>`
5. Crie um Pull Request.

Como alternativa, consulte a documentação do GitHub em [como criar uma solicitação pull](https://help.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request).
