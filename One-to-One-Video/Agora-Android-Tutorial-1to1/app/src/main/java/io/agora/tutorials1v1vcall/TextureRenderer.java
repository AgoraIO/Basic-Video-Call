package io.agora.tutorials1v1vcall;

import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.Matrix;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

public class TextureRenderer {
    private static final String LOG_TAG = "TextureRenderer";

    private final String mVertexShader =
            "attribute vec4 position;\n" +
                    "attribute vec2 inputTextureCoordinate;\n" +
                    "uniform mat4 uMVPMatrix;\n" +
                    "uniform mat4 uSTMatrix;\n" +
                    "varying vec2 textureCoordinate;\n" +
                    "void main()\n" +
                    "{\n" +
                    "gl_Position = uMVPMatrix * position;\n" +
                    "vec4 tex4 = vec4(inputTextureCoordinate.xy, 1.0, 1.0);\n" +
                    "textureCoordinate = (uSTMatrix * tex4).xy;\n" +
                    "}";

    private final String mFragmentShaderOes =
            "#extension GL_OES_EGL_image_external : require\n" +
                    "precision mediump float;\n" +
                    "varying vec2 textureCoordinate;\n" +
                    "uniform samplerExternalOES s_texture;\n" +
                    "void main() {\n" +
                    "gl_FragColor = texture2D(s_texture, textureCoordinate);\n" +
                    "}";

    private final String mFragmentShaderRgba =
            "precision mediump float;\n" +
                    "varying vec2 textureCoordinate;\n" +
                    "uniform sampler2D s_texture;\n" +
                    "void main() {\n" +
                    "gl_FragColor = texture2D(s_texture, textureCoordinate);\n" +
                    "}";

    private FloatBuffer vertexBuffer, textureVerticesBuffer;
    private ShortBuffer drawListBuffer;
    private final int mProgram;

    private boolean mOesTexture = true;

    // number of coordinates per vertex in this array
    static final int COORDS_PER_VERTEX = 2;
    static float squareVertices[] = {-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f};
    // in counterclockwise order:
    float textureVertices[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,};

    private short drawOrder[] = {0, 1, 2, 0, 2, 3}; // order to draw vertices

    private final int vertexStride = COORDS_PER_VERTEX * 4; // 4 bytes per vertex

    // uniforms
    private int muMVPMatrixHandle;
    private int muSTMatrixHandle;
    private float[] mMVPMatrix = new float[16];
    private float[] mSTMatrix = new float[16];

    public TextureRenderer(boolean isOesTexture) {
        mOesTexture = isOesTexture;

        ByteBuffer bb = ByteBuffer.allocateDirect(squareVertices.length * 4);
        bb.order(ByteOrder.nativeOrder());
        vertexBuffer = bb.asFloatBuffer();
        vertexBuffer.put(squareVertices);
        vertexBuffer.position(0);

        ByteBuffer dlb = ByteBuffer.allocateDirect(drawOrder.length * 2);
        dlb.order(ByteOrder.nativeOrder());
        drawListBuffer = dlb.asShortBuffer();
        drawListBuffer.put(drawOrder);
        drawListBuffer.position(0);

        ByteBuffer bb2 = ByteBuffer.allocateDirect(textureVertices.length * 4);
        bb2.order(ByteOrder.nativeOrder());
        textureVerticesBuffer = bb2.asFloatBuffer();
        textureVerticesBuffer.put(textureVertices);
        textureVerticesBuffer.position(0);

        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, mVertexShader);
        int fragmentShader;
        if (mOesTexture) {
            fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, mFragmentShaderOes);
        } else {
            fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, mFragmentShaderRgba);
        }

        mProgram = GLES20.glCreateProgram();             // create empty OpenGL ES Program
        GLES20.glAttachShader(mProgram, vertexShader);   // add the vertex shader to program
        GLES20.glAttachShader(mProgram, fragmentShader); // add the fragment shader to program
        GLES20.glLinkProgram(mProgram);

        muMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");
        muSTMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uSTMatrix");

        Matrix.setIdentityM(mMVPMatrix, 0);
        Matrix.setIdentityM(mSTMatrix, 0);
    }

    public void rotate(int degrees) {
        double theta = (double) degrees / 180 * Math.PI;
        mMVPMatrix[0] = (float) Math.cos(theta);
        mMVPMatrix[1] = -(float) Math.sin(theta);
        mMVPMatrix[4] = (float) Math.sin(theta);
        mMVPMatrix[5] = (float) Math.cos(theta);
    }

    /**
     * image mirror
     * @param x horizontal flip
     * @param y vertical flip
     */
    public void flip(boolean x, boolean y) {
        if (x || y) {
            Matrix.scaleM(mMVPMatrix, 0, x ? -1 : 1, y ? -1 : 1, 1);
        }
    }

    public void draw(int texture) {
        GLES20.glUseProgram(mProgram);

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        if (mOesTexture)
            GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, texture);
        else
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texture);

        int positionHandle = GLES20.glGetAttribLocation(mProgram, "position");
        GLES20.glEnableVertexAttribArray(positionHandle);
        GLES20.glVertexAttribPointer(positionHandle, COORDS_PER_VERTEX, GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);

        int textureCoordHandle = GLES20.glGetAttribLocation(mProgram, "inputTextureCoordinate");
        GLES20.glEnableVertexAttribArray(textureCoordHandle);
        GLES20.glVertexAttribPointer(textureCoordHandle, COORDS_PER_VERTEX, GLES20.GL_FLOAT, false, vertexStride, textureVerticesBuffer);

        GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
        GLES20.glUniformMatrix4fv(muSTMatrixHandle, 1, false, mSTMatrix, 0);

        GLES20.glDrawElements(GLES20.GL_TRIANGLES, drawOrder.length, GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

        // Disable vertex array
        GLES20.glDisableVertexAttribArray(positionHandle);
        GLES20.glDisableVertexAttribArray(textureCoordHandle);

        GLES20.glUseProgram(0);
    }

    private void printMatrix(float[] matrix) {
        // 4 x 4
        for (int i = 0; i < 4; i++) {
            Log.d(LOG_TAG, matrix[i * 4 + 0] + " " + matrix[i * 4 + 1] + " " + matrix[i * 4 + 2] + " " + matrix[i * 4 + 3]);
        }
    }

    public void draw(int texture, float[] matrix) {
//        Log.i(TAG, "Matrix is");
//        printMatrix(matrix);
//        mSTMatrix = matrix;
        for (int i = 0; i < matrix.length; i++) {
            mSTMatrix[i] = matrix[i];
        }
        // debug

        draw(texture);
    }

    private int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);

        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        return shader;
    }
}
