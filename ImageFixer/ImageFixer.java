import java.awt.image.BufferedImage;
import java.awt.Color;
import java.io.File;
import javax.imageio.ImageIO;
import java.util.Scanner;

public class ImageFixer{

     public static void main(String []args){
        
        System.out.println("Please enter alpha value:");
        Scanner sc = new Scanner(System.in);
        int alpha = sc.nextInt();
        System.out.println("Please enter file name:");
        String name = sc.next();
        sc.close();
        
        BufferedImage img = null;
        try{
            img = ImageIO.read(new File(name+".png"));
        }catch(Exception pEx){
            pEx.printStackTrace();
        }
        for(int x=0; x<img.getWidth(); x++){
            for(int y=0; y<img.getHeight(); y++){
                Color c = new Color(img.getRGB(x, y), true);
                if(c.getAlpha() < alpha){
                    img.setRGB(x, y, 0);
                }
            }
        }
        
        try{
            ImageIO.write(img, "PNG", new File(name+"2.png"));
        }catch(Exception pEx){
            pEx.printStackTrace();
        }
        
     }
}