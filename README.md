# Mahou Engine
Engine simples 2D/3D baseada em SDL2/OpenGL

- [x] Exibição de sprites
- [x] Animação de Sprites
- [x] Sprites com Alpha
- [x] Transformações Afins
- [x] Co-rotinas
- [x] Câmera
- [x] Entidade de jogo
- [x] Cena
- [x] Exibição de texto
- [x] Colisão Circle - Circle
- [x] Materiais
- [x] Instancing
- [ ] Imagens na camada UI
- [ ] Colisão Circle - Rect
- [ ] Colisão Rect - Rect (AABB)
- [ ] Modelos 3D


## Instalação

Instale qualquer ferramenta que possibilite a utilização do make, de preferência o [msys2](https://www.msys2.org/).

## Como Buildar

Estando na pasta que contém o Makefile, execute:
```
make res
```
Tal comando irá compilar headers contendo os binários referentes a todos
os aquivos pngs (Caso seja necessário compilar outras extensões basta adicionar a regra no Makefile).

E depois:
```
make
```
Tal comando irá compilar todos os projetos que estiverem na pasta [examples](examples/),
criando uma pasta **build** contendo o executável e as dlls necessárias.

![movement](bit/mahou.gif)
