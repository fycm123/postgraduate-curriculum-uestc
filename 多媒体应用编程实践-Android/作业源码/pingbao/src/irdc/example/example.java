package irdc.example;

import irdc.example.R;

import java.util.Date;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;


public class example extends Activity
{
  private TextView mTextView01;
  private ImageView mImageView01;
  
  /* LayoutInflater对象作为新建AlertDialog之用 */
  private LayoutInflater mInflater01;
  
  /* 输入解锁的View */
  private View mView01;
  private EditText mEditText01,mEditText02;
  
  /* menu选项identifier，用以识别事件 */
  static final private int MENU_ABOUT = Menu.FIRST;
  static final private int MENU_EXIT = Menu.FIRST+1;
  private Handler mHandler01 = new Handler();
  private Handler mHandler02 = new Handler();
  private Handler mHandler03 = new Handler();
  private Handler mHandler04 = new Handler();
  /* 控制User静止与否的Counter */
  private int intCounter1, intCounter2;
  /* 控制FadeIn与Fade Out的Counter */
  private int intCounter3, intCounter4;
  /* 控制循序替换背景图ID的Counter  */
  private int intDrawable=0;
  /* 上一次User有动作的Time Stamp */
  private Date lastUpdateTime;
  /* 计算User共几秒没有动作 */
  private long timePeriod;
  /* 静止超过n秒将自动进入屏幕保护 */
  private float fHoldStillSecond = (float) 5;
  private boolean bIfRunScreenSaver;
  private boolean bFadeFlagOut, bFadeFlagIn = false;
  private long intervalScreenSaver = 1000;
  private long intervalKeypadeSaver = 1000;
  private long intervalFade = 100;
  private int screenWidth, screenHeight;
  /* 每n秒置换图片 */
  private int intSecondsToChange = 5;
  
  /* 设置Screen Saver需要用到的背景图 */
  private static int[] screenDrawable = new int[]
  {
    R.drawable.pingbao1,
    R.drawable.pingbao2,
    R.drawable.pingbao3,
    R.drawable.pingbao4,
    R.drawable.pingbao5
  };
  
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    
    /* 必须在setContentView之前调用全屏幕显示 */
    requestWindowFeature(Window.FEATURE_NO_TITLE);
    getWindow().setFlags
    (
      WindowManager.LayoutParams.FLAG_FULLSCREEN,
      WindowManager.LayoutParams.FLAG_FULLSCREEN
    );
    setContentView(R.layout.main);
    
    /* onCreate all Widget */
    mTextView01 = (TextView)findViewById(R.id.myTextView1);
    mImageView01 = (ImageView)findViewById(R.id.myImageView1);
    mEditText01 = (EditText)findViewById(R.id.myEditText1);
    
    /* 初始取得User触碰手机的时间 */
    lastUpdateTime = new Date(System.currentTimeMillis());
    
    /* 初始化Layout上的Widget可见性 */
    recoverOriginalLayout();
  }
  
  @Override
  public boolean onCreateOptionsMenu(Menu menu)
  {
    // TODO Auto-generated method stub
    
    /* menu群组ID */
    int idGroup1 = 0;
    
    /* The order position of the item */
    int orderMenuItem1 = Menu.NONE;
    int orderMenuItem2 = Menu.NONE+1;
    
    /* 创建具有SubMenu的menu */
    menu.add
    (
      idGroup1, MENU_ABOUT, orderMenuItem1, R.string.app_about
    );
    /* 创建退出Menu */

    menu.add(idGroup1, MENU_EXIT, orderMenuItem2, R.string.str_exit);
    menu.setGroupCheckable(idGroup1, true, true);
    
    return super.onCreateOptionsMenu(menu);
  }
  
  @Override
  public boolean onOptionsItemSelected(MenuItem item)
  {
    // TODO Auto-generated method stub
    switch(item.getItemId())
    {
      case (MENU_ABOUT):
        new AlertDialog.Builder
        (
          example.this
        ).setTitle(R.string.app_about).setIcon
        (
          R.drawable.hippo
        ).setMessage
        (
          R.string.app_about_msg
        ).setPositiveButton(R.string.str_ok,
        new DialogInterface.OnClickListener()
        {
          public void onClick
          (DialogInterface dialoginterface, int i)
          {
          }
        }).show();
        break;
      case (MENU_EXIT):
        /* 离开程序 */
        finish();
        break;
    }
    return super.onOptionsItemSelected(item);
  }
  
  /* 监控User没有动作的运行线程 */
  private Runnable mTasks01 = new Runnable() 
  {
    public void run() 
    {
      intCounter1++;
      Date timeNow = new Date(System.currentTimeMillis());
      
      /* 计算User静止不动作的时间间距 */
      timePeriod =
      (long)timeNow.getTime() - (long)lastUpdateTime.getTime();
      
      float timePeriodSecond = ((float)timePeriod/1000);
      
      /* 如果超过时间静止不动 */
      if(timePeriodSecond>fHoldStillSecond)
      {
        /* 静止超过时间第一次的标记 */
        if(bIfRunScreenSaver==false)
        {
          /* 启动运行线程2 */
          mHandler02.postDelayed(mTasks02, intervalScreenSaver);
          
          /* Fade Out*/
          if(intCounter1%(intSecondsToChange)==0)
          {
            bFadeFlagOut=true;
            mHandler03.postDelayed(mTasks03, intervalFade);
          }
          else
          {
            /* 在Fade Out后立即Fade In */
            if(bFadeFlagOut==true)
            {
              bFadeFlagIn=true;
              mHandler04.postDelayed(mTasks04, intervalFade);
            }
            else
            {
              bFadeFlagIn=false;
              intCounter4 = 0;
              mHandler04.removeCallbacks(mTasks04);
            }
            intCounter3 = 0;
            bFadeFlagOut = false;
          }
          bIfRunScreenSaver = true;
        }
        else
        {
          /* screen saver 正在运行中 */
          
          /* Fade Out*/
          if(intCounter1%(intSecondsToChange)==0)
          {
            bFadeFlagOut=true;
            mHandler03.postDelayed(mTasks03, intervalFade);
          }
          else
          {
            /* 在Fade Out后立即Fade In */
            if(bFadeFlagOut==true)
            {
              bFadeFlagIn=true;
              mHandler04.postDelayed(mTasks04, intervalFade);
            }
            else
            {
              bFadeFlagIn=false;
              intCounter4 = 0;
              mHandler04.removeCallbacks(mTasks04);
            }
            intCounter3 = 0;
            bFadeFlagOut=false;
          }
        }
      }
      else
      {
        /* 当User没有动作的间距未超过时间 */
        bIfRunScreenSaver = false;
        /* 恢复原来的Layout Visible*/
        recoverOriginalLayout();
      }
      
      /* 以LogCat监看User静止不动的时间间距 */
      Log.i
      (
        "HIPPO",
        "Counter1:"+Integer.toString(intCounter1)+
        "/"+
        Float.toString(timePeriodSecond));
      
      /* 反复运行运行线程1 */
      mHandler01.postDelayed(mTasks01, intervalKeypadeSaver);
    } 
  };
  
  /* Screen Saver Runnable */
  private Runnable mTasks02 = new Runnable() 
  {
    public void run() 
    {
      if(bIfRunScreenSaver==true)
      {
        intCounter2++;
        
        hideOriginalLayout();
        showScreenSaver();
        
        //Log.i("HIPPO", "Counter2:"+Integer.toString(intCounter2));
        mHandler02.postDelayed(mTasks02, intervalScreenSaver);
      }
      else
      {
        mHandler02.removeCallbacks(mTasks02);
      }
    } 
  };
  
  /* Fade Out特效Runnable */
  private Runnable mTasks03 = new Runnable() 
  {
    public void run() 
    {
      if(bIfRunScreenSaver==true && bFadeFlagOut==true)
      {
        intCounter3++;
        
        /* 设置ImageView的透明度渐暗下去 */
        mImageView01.setAlpha(255-intCounter3*28);
        
        Log.i("HIPPO", "Fade out:"+Integer.toString(intCounter3));
        mHandler03.postDelayed(mTasks03, intervalFade);
      }
      else
      {
        mHandler03.removeCallbacks(mTasks03);
      }
    } 
  };
  
  /* Fade In特效Runnable */
  private Runnable mTasks04 = new Runnable() 
  {
    public void run() 
    {
      if(bIfRunScreenSaver==true && bFadeFlagIn==true)
      {
        intCounter4++;
        
        /* 设置ImageView的透明度渐亮起来 */
        mImageView01.setAlpha(intCounter4*28);
        
        mHandler04.postDelayed(mTasks04, intervalFade);
        Log.i("HIPPO", "Fade In:"+Integer.toString(intCounter4));
      }
      else
      {
        mHandler04.removeCallbacks(mTasks04);
      }
    } 
  };
  
  /* 恢复原有的Layout可视性 */
  private void recoverOriginalLayout()
  {
    mTextView01.setVisibility(View.VISIBLE);
    mEditText01.setVisibility(View.VISIBLE);
    mImageView01.setVisibility(View.GONE);
  }
  
  /* 隐藏原有应用程序里的布局配置组件 */
  private void hideOriginalLayout()
  {
    /* 将欲隐藏的Widget写在此 */
    mTextView01.setVisibility(View.INVISIBLE);
    mEditText01.setVisibility(View.INVISIBLE);
  }
  
  /* 开始ScreenSaver */
  private void showScreenSaver()
  {
    /* 屏幕保护之后要做的事件写在此*/
    
    if(intDrawable>4)
    {
      intDrawable = 0;
    }
    
    DisplayMetrics dm=new DisplayMetrics();
    getWindowManager().getDefaultDisplay().getMetrics(dm);
    screenWidth = dm.widthPixels;
    screenHeight = dm.heightPixels;
    Bitmap bmp=BitmapFactory.decodeResource(getResources(), screenDrawable[intDrawable]);
    
    /* Matrix比例 */ 
    float scaleWidth = ((float) screenWidth) / bmp.getWidth();
    float scaleHeight = ((float) screenHeight) / bmp.getHeight() ;
    
    Matrix matrix = new Matrix(); 
    /* 使用Matrix.postScale设置维度ReSize */ 
    matrix.postScale(scaleWidth, scaleHeight);
    
    /* ReSize图文件至屏幕分辨率 */
    Bitmap resizedBitmap = Bitmap.createBitmap
    (
      bmp,0,0,bmp.getWidth(),bmp.getHeight(),matrix,true
    );
    
    /* 新建Drawable放大图文件至全屏幕 */
    BitmapDrawable myNewBitmapDrawable = 
        new BitmapDrawable(resizedBitmap); 
    mImageView01.setImageDrawable(myNewBitmapDrawable);
    
    /* 使ImageView可见 */
    mImageView01.setVisibility(View.VISIBLE);
    
    /* 每间隔设置秒数置换图片ID，于下一个runnable2才会生效 */
    if(intCounter2%intSecondsToChange==0)
    {
      intDrawable++;
    }
  }
  
  public void onUserWakeUpEvent()
  {
    if(bIfRunScreenSaver==true)
    {
      try
      {
        /* LayoutInflater.from取得此Activity的context */
        mInflater01 = LayoutInflater.from(example.this);
        
        /* 创建解锁密码使用View的Layout */
        mView01 = mInflater01.inflate(R.layout.securescreen, null);
        
        /* 于对话框中唯一的EditText等待输入解锁密码 */
        mEditText02 =
        (EditText) mView01.findViewById(R.id.myEditText2);
        
        /* 创建AlertDialog */
        new AlertDialog.Builder(this)
        .setView(mView01)
        .setPositiveButton("OK",
        new DialogInterface.OnClickListener()
        {
          public void onClick(DialogInterface dialog, int whichButton)
          {
            /* 比较输入的密码与原Activity里的设置是否相符 */
            if(mEditText01.getText().toString().equals
              (mEditText02.getText().toString()))
            {
              /* 当密码正确才真的解锁屏幕保护装置 */
              resetScreenSaverListener();
            }
          }
        }).show();
      }
      catch(Exception e)
      {
        e.printStackTrace();
      }
    }
  }

  
  public void updateUserActionTime()
  {
    /* 取得点击按键事件时的系统Time Millis */
    Date timeNow = new Date(System.currentTimeMillis());
    
    /* 重新计算点击按键距离上一次静止的时间间距 */
    timePeriod =
    (long)timeNow.getTime() - (long)lastUpdateTime.getTime();
    lastUpdateTime.setTime(timeNow.getTime());
  }
  
  public void resetScreenSaverListener()
  {
    /* 删除现有的Runnable */
    mHandler01.removeCallbacks(mTasks01);
    mHandler02.removeCallbacks(mTasks02);
    
    /* 取得点击按键事件时的系统Time Millis */
    Date timeNow = new Date(System.currentTimeMillis());
    /* 重新计算点击按键距离上一次静止的时间间距 */
    timePeriod =
    (long)timeNow.getTime() - (long)lastUpdateTime.getTime();
    lastUpdateTime.setTime(timeNow.getTime());
    
    /* for Runnable2，取消屏幕保护 */
    bIfRunScreenSaver = false;
    
    /* 重置Runnable1与Runnable1的Counter */
    intCounter1 = 0;
    intCounter2 = 0;
    
    /* 恢复原来的Layout Visible*/
    recoverOriginalLayout();
    
    /* 重新postDelayed()新的Runnable */
    mHandler01.postDelayed(mTasks01, intervalKeypadeSaver);
  }
  
  @Override
  public boolean onKeyDown(int keyCode, KeyEvent event)
  {
    // TODO Auto-generated method stub
    if(bIfRunScreenSaver==true && keyCode!=4)
    {
      /* 当屏幕保护程序正在运行中，触动解除屏幕保护程序 */
      onUserWakeUpEvent();
    }
    else
    {
      /* 更新User未触动手机的时间戳记 */
      updateUserActionTime();
    }
    return super.onKeyDown(keyCode, event);
  }
  @Override
  public boolean onTouchEvent(MotionEvent event)
  {
    // TODO Auto-generated method stub
    if(bIfRunScreenSaver==true)
    {
      /* 当屏幕保护程序正在运行中，触动解除屏幕保护程序 */
      onUserWakeUpEvent();
    }
    else
    {
      /* 更新User未触动手机的时间戳记 */
      updateUserActionTime();
    }
    return super.onTouchEvent(event);
  }
  
  @Override
  protected void onResume()
  {
    // TODO Auto-generated method stub
    mHandler01.postDelayed(mTasks01, intervalKeypadeSaver);
    super.onResume();
  }
  
  @Override
  protected void onPause()
  {
    // TODO Auto-generated method stub
    
    try
    {
      /* 删除运行中的运行线程 */
      mHandler01.removeCallbacks(mTasks01);
      mHandler02.removeCallbacks(mTasks02);
      mHandler03.removeCallbacks(mTasks03);
      mHandler04.removeCallbacks(mTasks04);
    }
    catch(Exception e)
    {
      e.printStackTrace();
    }
    super.onPause();
  }
}
