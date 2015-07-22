package com.example.camerauseintent;

import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.Images.Media;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

public class MainActivity extends ActionBarActivity {


	private static final int CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE = 100;
	private static final int CAPTURE_VIDEO_ACTIVITY_REQUEST_CODE = 200;
	public static final int MEDIA_TYPE_IMAGE = 1;
	public static final int MEDIA_TYPE_VIDEO = 2;
	public byte recv_buffer [] = new byte[6];
	public String server_ip;
	private Uri fileUri;
	private Button TakePhotoButton;
	private Button SnapButton;
	private EditText ipEdit;
	private ImageView myImageView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		//�����հ�ť�ļ�����
		TakePhotoButton = (Button)findViewById(R.id.TakePhotoButton);
		TakePhotoButton.setOnClickListener(new MyButtonListener1());
		//��socket��ť�ļ�����
		SnapButton = (Button)findViewById(R.id.SnapButton);
		SnapButton.setOnClickListener(new MyButtonListener2());
		
		ipEdit = (EditText)findViewById(R.id.ipEdit);
	}
	
	class MyButtonListener1 implements OnClickListener{
		@Override
		public void onClick(View v) {
			// create Intent to take a picture and return control to the calling application
		    Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		    fileUri = getOutputMediaFileUri(MEDIA_TYPE_IMAGE); // create a file to save the image
		    intent.putExtra(MediaStore.EXTRA_OUTPUT, fileUri); // set the image file name
		    startActivityForResult(intent, CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE);
		}
	}
	
	class MyButtonListener2 implements OnClickListener{
		@Override
		public void onClick(View v) {
			Toast.makeText(getApplicationContext(), "Snaping...Please wait ^_^",Toast.LENGTH_LONG).show();
			
			Thread t = new ServerThread();
			t.start();
			/*
			try {
				t.join();//����ǰ�̣߳�ֱ��������ϲż���ִ������Ĳ���
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			*/
		}
	}
	
	class ServerThread extends Thread{
    	public void run(){
    		try {
    			//��������socket�߳�������ͼƬ������ȡ���صĽ��
    			
    			//����һ��Socket����ָ���������˵�IP��ַ�Ͷ˿ں�
    			server_ip = ipEdit.getText().toString();
    			Socket socket = new Socket(server_ip,4567);
    			//ʹ��InputStream��ȡӲ���ϵ��ļ�
    			InputStream inputStream = getContentResolver().openInputStream(fileUri);
    			OutputStream outputStream = socket.getOutputStream();
    			
    			//���ȷ���ͼƬ�Ĵ�С������Ҫ��int��char��ת��
    			int temp = 0 ;
    			int img_size = inputStream.available();
    			String img_size_str = String.valueOf(img_size) + "\n";//��\n��Ϊ�����ַ����Ľ������ 
    			outputStream.write(img_size_str.getBytes(), 0, img_size_str.length());
    			
    			//Ȼ��������ͼƬ
    			byte img_buffer [] = new byte[1024 * 1024 * 3];
    			temp = 0;
    			//��InputStream���е�����ȡ������д�뵽OutputStream����
    			while((temp = inputStream.read(img_buffer)) != -1){
    				outputStream.write(img_buffer, 0, temp);
    			}
    			outputStream.flush();
    			
    			
    			//��Socket���еõ�InputStream����
				inputStream = socket.getInputStream();
				temp = 0;
				//��InputStream���ж�ȡ�ͻ��������͵�����
				while((temp = inputStream.read(recv_buffer)) != -1){
					System.out.println(new String(recv_buffer,0,temp));
				}
				
				//�߳̽���֮�󣬴�MainActivity��ResultActivity
				Intent intent = new Intent();
				//�Ӹ�Activityͨ����Intent����Data�����ݸ�����һ��Activity������ʽ��key-value
				String result = new String(recv_buffer, 0, 5);
				intent.putExtra("result", result);
				intent.setClass(MainActivity.this, ResultActivity.class);
				MainActivity.this.startActivity(intent);
				
    		} catch (Exception e) {
    			// TODO Auto-generated catch block
    			e.printStackTrace();
    		}
    	}
	}
	
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	    if (requestCode == CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE) {
	        if (resultCode == RESULT_OK) {
	            // Image captured and saved to fileUri specified in the Intent
	            Toast.makeText(getApplicationContext(), "Image saved to:\n" +
	            		fileUri.getPath(), Toast.LENGTH_LONG).show();
	        } 
	        /*
	        else if (resultCode == RESULT_CANCELED) {
	            // User cancelled the image capture
	        } else {
	            // Image capture failed, advise user
	             */
	        	//Bitmap photo = (Bitmap) data.getExtras().get("data");
        	try
        	{
        		Bitmap photo = Media.getBitmap(this.getContentResolver(), fileUri);
	        	myImageView = (ImageView)findViewById(R.id.myImageView);
	        	myImageView.setImageBitmap(photo);
        	}
        	catch(Exception e)
        	{
        		
        	}
	    }

	    if (requestCode == CAPTURE_VIDEO_ACTIVITY_REQUEST_CODE) {
	        if (resultCode == RESULT_OK) {
	            // Video captured and saved to fileUri specified in the Intent
	            //Toast.makeText(this, "Video saved to:\n" +
	            //        data.getData(), Toast.LENGTH_LONG).show();
	        } else if (resultCode == RESULT_CANCELED) {
	            // User cancelled the video capture
	        } else {
	            // Video capture failed, advise user
	        }
	    }
	}
	
	/* Checks if external storage is available for read and write */
	public static boolean isExternalStorageWritable() {
	    String state = Environment.getExternalStorageState();
	    if (Environment.MEDIA_MOUNTED.equals(state)) {
	        return true;
	    }
	    return false;
	}
	
	/** Create a file Uri for saving an image or video */
	private static Uri getOutputMediaFileUri(int type){
	      return Uri.fromFile(getOutputMediaFile(type));
	}
	
	/** Create a File for saving an image or video */
	private static File getOutputMediaFile(int type){
	    // To be safe, you should check that the SDCard is mounted
	    // using Environment.getExternalStorageState() before doing this.
		if(!isExternalStorageWritable())
			 Log.d("Leafsnap", "the SD card is unwriteable!!");
		
	    File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(
	              Environment.DIRECTORY_PICTURES), "Leafsnap");
	    // This location works best if you want the created images to be shared
	    // between applications and persist after your app has been uninstalled.
	    Log.d("Leafsnap", mediaStorageDir.getAbsolutePath());
	    // Create the storage directory if it does not exist
	    if (! mediaStorageDir.exists()){
	        if (! mediaStorageDir.mkdirs()){
	            Log.d("Leafsnap", "failed to create directory");
	            return null;
	        }
	    }

	    // Create a media file name
	    String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
	    File mediaFile;
	    if (type == MEDIA_TYPE_IMAGE){
	        mediaFile = new File(mediaStorageDir.getPath() + File.separator +
	        "IMG_"+ timeStamp + ".jpg");
	    } else if(type == MEDIA_TYPE_VIDEO) {
	        mediaFile = new File(mediaStorageDir.getPath() + File.separator +
	        "VID_"+ timeStamp + ".mp4");
	    } else {
	        return null;
	    }

	    return mediaFile;
	}


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
