/*
 * DCE05968 - Estruturas de Dados I
 * Funções responsaveis pelos filtros que podem ser aplicados na imagem.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Imagem.h"  /* Tipos e protótipos de funções que manipulam imagens */
#include "Filtros.h" /* Protótipo das funções responsáveis pelos filtros das imagens */

/* Função responsável por escurecer uma imagem
 * A função recebe um ponteiro para um struct Imagem e altera/escurece 
 * cada um de seus pixels em cada canal de cor (RGB).
 * 
 * Analise essa função com bastante atenção. Ela irá te auxiliar no entendimento 
 * do TAD Imagem e como outros filtros podem ser implementados.
 *
 * Você não precisa modificar essa função 
 */
void escurecerImagem(Imagem *img){
    int v;
    printf("Digite o fator de escurecimento: ");
    scanf("%d", &v);
    /* Cada canal de cor (RGB) de cada pixel é reduzido 'v' do valor.
     * Note que devemos garantir que o valor esteja entre 0 e 255.   
     * Como estamos subtraindo, verificamos apenas se o valor é >= 0 
     * Note também a utilização de expressão ternária e o cast (conversão) 
     * entre os valores Byte (unsigned int) e int. Esse cast evita erros nas 
     * operações matemáticas.
     */ 
    for (int h = 0; h < obtemAltura(img); h++) {
        for (int w = 0; w < obtemLargura(img); w++) {
            //Obtém o pixel da posição (h, w) da imagem 
            Pixel pixel = obtemPixel(img, h, w);
            //Modifica cada canal de cor do pixel
            pixel.cor[RED]   = (((int)pixel.cor[RED] - v) >= 0 ? ((int)pixel.cor[RED] - v) : 0);
            pixel.cor[GREEN] = (((int)pixel.cor[GREEN] - v) >= 0 ? ((int)pixel.cor[GREEN] - v) : 0);
            pixel.cor[BLUE]  = (((int)pixel.cor[BLUE] - v) >= 0 ? ((int)pixel.cor[BLUE] - v) : 0);
            //Grava o novo pixel na posição (h, w) da imagem
            recolorePixel(img, h, w, pixel);
        }
    }
}

/* Função responsável por clarear uma imagem
 * A função recebe um ponteiro para um struct Imagem e altera/clareia 
 * cada um de seus pixels em cada canal de cor (RGB).
 */
void clarearImagem(Imagem *img){
    int v;
    printf("Digite o fator de clareamento: ");
    scanf("%d", &v);

    for (int h = 0; h < obtemAltura(img); h++) {
        for (int w = 0; w < obtemLargura(img); w++) {
            //Obtém o pixel da posição (h, w) da imagem 
            Pixel pixel = obtemPixel(img, h, w);
            //Modifica cada canal de cor do pixel
            pixel.cor[RED]   = (((int)pixel.cor[RED] + v) <= 255 ? ((int)pixel.cor[RED] + v) : 255);
            pixel.cor[GREEN] = (((int)pixel.cor[GREEN] + v) <= 255 ? ((int)pixel.cor[GREEN] + v) : 255);
            pixel.cor[BLUE]  = (((int)pixel.cor[BLUE] + v) <= 255 ? ((int)pixel.cor[BLUE] + v) : 255);
            //Grava o novo pixel na posição (h, w) da imagem
            recolorePixel(img, h, w, pixel);
        }
    }
}

/* Função responsável por converter uma imagem colorida em escala de cinza
 * A função recebe um ponteiro para um struct Imagem e converte 
 * cada um de seus pixels em cada canal de cor (RGB) em uma tonalidade de cinza.
 * 
 * No pdf de descrição do EP existem duas estretégias para a implementação 
 * desse filtro. Você deve implementar uma delas ou pesquisar por outras alternativas. 
 */
void escalaDeCinzaImagem(Imagem *img){
    int media;
    for (int h = 0; h < obtemAltura(img); h++) {
        for (int w = 0; w < obtemLargura(img); w++) {
            //Obtém o pixel da posição (h, w) da imagem 
            Pixel pixel = obtemPixel(img, h, w);
            media = 0.3 * (int)pixel.cor[RED] + 0.59 * (int)pixel.cor[GREEN] + 0.11 * (int)pixel.cor[BLUE]/3;

            pixel.cor[RED] = media;
            pixel.cor[GREEN] = media;
            pixel.cor[BLUE] = media;

            recolorePixel(img, h, w, pixel);
        }
    }
}

/* Função responsável por aplicar o filtro de Sobel na imagem. 
 * Leia o pdf do EP para obter mais informações de como o filtro é implementado. 
 */
void filtroSobel(Imagem *img){
    
    /* Dicas:
     * 1) Se quiser, pode ignorar as bordas das imagens
     * 2) Para o pixel na posição (h, w), obtenha os outros 8 pixels vizinhos e aplique a matriz gx e gy 
     *    em cada banda de cor do pixel. Combine o valor obtido por gx e gy e modifique o pixel de img.
     *    Lembre-se que a obtenção dos pixels deve ser a partir de uma cópia da imagem. 
     * 3) Verifique se o novo valor obtido para a banda de cor é um valor válido (entre 0 e 255).
     */
    int largura = obtemLargura(img);
    int altura = obtemAltura(img);
    Imagem *copia = copiaImagem(img);
    for (int h = 1; h < altura - 1; h++) {
        for (int w = 1; w < largura - 1; w++) {

            int redx = 0;
            int greenx = 0;
            int bluex = 0;
            int redy = 0;
            int greeny = 0;
            int bluey = 0;

            //Obtém o pixel da posição (h, w) da imagem 
            //primeiro i - 1; j - 1  :: pesox 1 :: pesoy 1
            Pixel pixel = obtemPixel(copia, h - 1, w - 1);
            redx = redx + (int)pixel.cor[RED]; 
            greenx = greenx + (int)pixel.cor[GREEN];
            bluex = bluex + (int)pixel.cor[BLUE]; 

            redy += (int)pixel.cor[RED]; 
            greeny += (int)pixel.cor[GREEN];
            bluey += (int)pixel.cor[BLUE];
            
            //segundo i - 1; j  :: pesox 2
            
            pixel = obtemPixel(copia, h - 1, w);
            redx += 2 * (int)pixel.cor[RED]; 
            greenx += 2 * (int)pixel.cor[GREEN];
            bluex += 2 * (int)pixel.cor[BLUE]; 
            
            //terceiro i - 1; j + 1  :: pesox 1 :: pesoy -1
            
            pixel = obtemPixel(copia, h - 1, w + 1);
            redx += (int)pixel.cor[RED]; 
            greenx += (int)pixel.cor[GREEN];
            bluex += (int)pixel.cor[BLUE]; 

            redy -= (int)pixel.cor[RED]; 
            greeny -= (int)pixel.cor[GREEN];
            bluey -= (int)pixel.cor[BLUE];
        

            //quarto i + 1; j - 1  :: pesox -1 pesoy 1
            
            pixel = obtemPixel(copia, h + 1, w - 1);
            redx -= (int)pixel.cor[RED]; 
            greenx -= (int)pixel.cor[GREEN];
            bluex -= (int)pixel.cor[BLUE];

            redy += (int)pixel.cor[RED]; 
            greeny += (int)pixel.cor[GREEN];
            bluey += (int)pixel.cor[BLUE];
        
            //quinto i + 1; j  :: pesox -2
            
            pixel = obtemPixel(copia, h + 1, w);
            redx -= 2 * (int)pixel.cor[RED]; 
            greenx -= 2 * (int)pixel.cor[GREEN];
            bluex -= 2 * (int)pixel.cor[BLUE];
            
            //sexto i + 1; j + 1  :: pesox -1 :: pesoy -1
            
            pixel = obtemPixel(copia, h - 1, w + 1);
            redx -= (int)pixel.cor[RED]; 
            greenx -= (int)pixel.cor[GREEN];
            bluex -= (int)pixel.cor[BLUE];

            redy -= (int)pixel.cor[RED]; 
            greeny -= (int)pixel.cor[GREEN];
            bluey -= (int)pixel.cor[BLUE];
            

            //setimo i; j - 1  :: pesoy = 2
            
            pixel = obtemPixel(copia, h, w - 1);
            redy += 2 * (int)pixel.cor[RED]; 
            greeny += 2 * (int)pixel.cor[GREEN];
            bluey += 2 * (int)pixel.cor[BLUE];
        
            //oitavo i; j + 1  :: pesoy = -2
            
            pixel = obtemPixel(copia, h, w + 1);
            redy -= 2 * (int)pixel.cor[RED]; 
            greeny -= 2 * (int)pixel.cor[GREEN];
            bluey -= 2 * (int)pixel.cor[BLUE];
        
            int novoRed, novoGreen, novoBlue;
            novoRed = redx + redy / 2;
            novoGreen = greenx + greeny / 2;
            novoBlue = bluex + bluey / 2;

            pixel = obtemPixel(img, h, w);
            pixel.cor[RED] = novoRed <= 0 ? 0 : novoRed >= 255 ? 255 : novoRed;
            pixel.cor[GREEN] = novoGreen <= 0 ? 0 : novoGreen >= 255 ? 255 : novoGreen;
            pixel.cor[BLUE] = novoBlue <= 0 ? 0 : novoBlue >= 255 ? 255 : novoBlue;
            recolorePixel(img, h, w, pixel);
        }
    }
    liberaImagem(copia);
}

/* Função responsável por aplicar a detecçõa de bordas de Laplace na imagem. 
 * Leia o pdf do EP para obter mais informações de como o filtro é implementado. 
 */
void deteccaoBordasLaplace(Imagem *img) {
    /* Siga as mesmas dicas do filtro de Sobel */

    int largura = obtemLargura(img);
    int altura = obtemAltura(img);
    Imagem *copia = copiaImagem(img);
    for (int h = 1; h < altura - 1; h++) {
        for (int w = 1; w < largura - 1; w++) {
            int red = 0;
            int green = 0;
            int blue = 0;

            //primeiro i - 1; j  :: peso -1
        
            Pixel pixel = obtemPixel(copia, h - 1, w);
            red -= (int)pixel.cor[RED]; 
            green -= (int)pixel.cor[GREEN];
            blue -= (int)pixel.cor[BLUE];

        
        //segundo i + 1; j  :: peso -1
        
            pixel = obtemPixel(copia, h + 1, w);
            red -= (int)pixel.cor[RED]; 
            green -= (int)pixel.cor[GREEN];
            blue -= (int)pixel.cor[BLUE];
        
        //terceiro i; j - 1  :: peso -1
        
            pixel = obtemPixel(copia, h, w - 1);
            red -= (int)pixel.cor[RED]; 
            green -= (int)pixel.cor[GREEN];
            blue -= (int)pixel.cor[BLUE];
        
        //quarto i; j + 1  :: peso -1
        
            pixel = obtemPixel(copia, h, w + 1);
            red -= (int)pixel.cor[RED]; 
            green -= (int)pixel.cor[GREEN];
            blue -= (int)pixel.cor[BLUE];
        
            //quinto i; j  :: peso 4
            pixel = obtemPixel(copia, h, w);
            red += 4 * (int)pixel.cor[RED]; 
            green += 4 * (int)pixel.cor[GREEN];
            blue += 4 * (int)pixel.cor[BLUE];

            Pixel pixelOriginal = obtemPixel(img, h, w);
            pixelOriginal.cor[RED] = red <= 0 ? 0 : red >= 255 ? 255 : red;
            pixelOriginal.cor[GREEN] = green <= 0 ? 0 : green >= 255 ? 255 : green;
            pixelOriginal.cor[BLUE] = blue <= 0 ? 0 : blue >= 255 ? 255 : blue;

            recolorePixel(img, h, w, pixelOriginal);
        }
    }
    liberaImagem(copia);
}

/* Função responsável por aplicar na imagem o filtro definido por você. Seja criativo! 
 * Filtros que apenas fazem algumas modificações simples em cada pixel 
 * (por exemplo, negativo, espelhar a imagem) não serão considerados.
 */


void meuFiltro(Imagem *img){ // destacar relevo
    // -2 -1  0 
    // -1  1  1
    //  0  1  2

    int largura = obtemLargura(img);
    int altura = obtemAltura(img);
    Imagem *copia = copiaImagem(img);
    for (int h = 1; h < altura - 1; h++) {
        for (int w = 1; w < largura - 1; w++) {
            int red = 0;
            int green = 0;
            int blue = 0;

            //primeiro i - 1; j - 1  :: peso -2
        
            Pixel pixel = obtemPixel(copia, h - 1, w - 1);
            red -= 2 * (int)pixel.cor[RED]; 
            green -= 2 * (int)pixel.cor[GREEN];
            blue -= 2 * (int)pixel.cor[BLUE];

        
        //segundo i - 1; j  :: peso -1
        
            pixel = obtemPixel(copia, h - 1, w);
            red -= (int)pixel.cor[RED]; 
            green -= (int)pixel.cor[GREEN];
            blue -= (int)pixel.cor[BLUE];
        
        //terceiro i; j - 1  :: peso -1
        
            pixel = obtemPixel(copia, h, w - 1);
            red -= (int)pixel.cor[RED]; 
            green -= (int)pixel.cor[GREEN];
            blue -= (int)pixel.cor[BLUE];
        
        //quarto i; j + 1  :: peso 1
        
            pixel = obtemPixel(copia, h, w + 1);
            red += (int)pixel.cor[RED]; 
            green += (int)pixel.cor[GREEN];
            blue += (int)pixel.cor[BLUE];
        
        //quinto i + 1; j  :: peso 1
        
            pixel = obtemPixel(copia, h + 1, w);
            red += (int)pixel.cor[RED]; 
            green += (int)pixel.cor[GREEN];
            blue += (int)pixel.cor[BLUE];
        
            //sexto i + 1;j + 1  :: peso 2
            pixel = obtemPixel(copia, h + 1, w + 1);
            red += 2 * (int)pixel.cor[RED]; 
            green += 2 * (int)pixel.cor[GREEN];
            blue += 2 * (int)pixel.cor[BLUE];

            //setimo i; j  :: peso 1
            pixel = obtemPixel(copia, h, w);
            red += (int)pixel.cor[RED]; 
            green += (int)pixel.cor[GREEN];
            blue += (int)pixel.cor[BLUE];
            

            Pixel pixelOriginal = obtemPixel(img, h, w);
            pixelOriginal.cor[RED] = red <= 0 ? 0 : red >= 255 ? 255 : red;
            pixelOriginal.cor[GREEN] = green <= 0 ? 0 : green >= 255 ? 255 : green;
            pixelOriginal.cor[BLUE] = blue <= 0 ? 0 : blue >= 255 ? 255 : blue;

            recolorePixel(img, h, w, pixelOriginal);
        }
    }
    liberaImagem(copia);   
}