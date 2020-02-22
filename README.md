# Labirinto 3D

O jogo consiste de um personagem andando por um labirinto à procura da saída.

### Desenvolvimento
* Leitura do modelo 3D “.obj” do personagem. Obs: O modelo 3D pode ser obtido em algum repositório (por exemplo, http://tf3dm.com/3d-models), i.e., construído no Blender ou outro software de modelagem 3D qualquer e exportar como formato “.obj”. Mais informações sobre o formato “.obj” em https://en.wikipedia.org/wiki/Wavefront_.obj_file ou http://www.martinreddy.net/gfx/3d/OBJ.spec
* Construção do labirinto 3D por onde o personagem vai caminhar. O modelo exportado “.obj” é carregado como arquivo no jogo. O labirinto tem vários caminhos sem saída e as paredes são mais “altas” que o personagem, de forma que o personagem não possa olhar “por cima” das paredes. (Técnica de modelagem de extrusão);
* Desenvolvimento das funções para o personagem andar pelo labirinto utilizando eventos de teclado ou mouse;
* Desenvolvimento do sistema de câmera que acompanha o personagem pelo labirinto numa visão em primeira pessoa;
* Inclusão de textura ao personagem e as paredes do labirinto;
* Fim do jogo: Quando o personagem sai do labirinto indica que ele cumpriu seu objetivo.