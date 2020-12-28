import java.io.*;

/**
 * Copy one file to another using low level byte streams, one byte at a time.
 * @author www.codejava.net
 */
public class CopyFiles {
    public static void main(String[] args) {
		if (args.length < 2) {
			System.out.println("Please provide input and output files");
			System.exit(0);
		}

		String inputFile = args[0];
		String outputFile = args[1];


        try (
			InputStream inputStream = new FileInputStream(inputFile);
			OutputStream outputStream = new FileOutputStream(outputFile);
        ) {

            int byteRead;

            while ((byteRead = inputStream.read()) != -1) {
                outputStream.write(byteRead);
            }

        } catch (IOException ex) {
			ex.printStackTrace();
		}
    }
}
