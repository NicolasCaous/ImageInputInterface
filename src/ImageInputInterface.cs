using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class ImageInputInterface : MonoBehaviour
{

    [DllImport("ImageInputInterface")]
    private static extern void AddBox(int id);

    [DllImport("ImageInputInterface")]
    private static extern void RemoveBox(int id);

    [DllImport("ImageInputInterface")]
    private static extern void Init(int mainThreadDeltaMs, int boxThreadDeltaMs, int blueThreshValue, int greenThreshValue, int redThreshValue);

    [DllImport("ImageInputInterface")]
    private static extern void Stop();

    [DllImport("ImageInputInterface")]
    private static extern int GetStatus();

    [DllImport("ImageInputInterface")]
    private static extern int GetMatType();

    [DllImport("ImageInputInterface")]
    private static extern double GetBoxValue(int id);

    [DllImport("ImageInputInterface")]
    private static extern void SetBaseValues();

    [DllImport("ImageInputInterface")]
    private static extern void GetRawImage(IntPtr data, int width, int height);

    [DllImport("ImageInputInterface")]
    private static extern void GetRawButtonMaskImage(int id, IntPtr data, int width, int height);

    [DllImport("ImageInputInterface")]
    private static extern void GetRawButtonImage(int id, IntPtr data, int width, int height);

    [DllImport("ImageInputInterface")]
    private static extern void GetMaskDetectionOutput(IntPtr data, int width, int height);

    [DllImport("ImageInputInterface")]
    public static extern int GetRawImageArrayLenght();
    [DllImport("ImageInputInterface")]
    public static extern int GetRawImageHeight();
    [DllImport("ImageInputInterface")]
    public static extern int GetRawImageWidth();

    public int mainThreadDeltaMs;
    public int boxThreadDeltaMs;

    public int blueThreshValue = 200;
    public int greenThreshValue = 200;
    public int redThreshValue = 200;

    void Awake()
    {
        Debug.Log("Initialized");
        Init(mainThreadDeltaMs, boxThreadDeltaMs, blueThreshValue, greenThreshValue, redThreshValue);
    }

    void OnApplicationQuit()
    {
        Debug.Log("Finished");
        Stop();
    }

    public static int GetStatusFromModule()
    {
        return GetStatus();
    }

    public static int GetMatTypeFromModule()
    {
        return GetMatType();
    }

    public static double GetBoxValueFromModule(int id)
    {
        return GetBoxValue(id);
    }

    public static void SetBaseValuesFromModule()
    {
        SetBaseValues();
    }

    public sealed class RawImageSession
    {

        private static RawImageSession instance = null;
        private static readonly object padlock = new object();
        private Texture2D tex;
        private Color32[] pixel32;
        private GCHandle pixelHandle;

        private RawImageSession()
        {
            this.tex = new Texture2D(
                ImageInputInterface.GetRawImageWidth(),
                ImageInputInterface.GetRawImageHeight(),
                TextureFormat.ARGB32,
                false
            );
            this.pixel32 = tex.GetPixels32();
            this.pixelHandle = GCHandle.Alloc(this.pixel32, GCHandleType.Pinned);
        }

        ~RawImageSession()
        {
            this.pixelHandle.Free();
        }

        public static RawImageSession GetInstance()
        {
            lock (padlock)
            {
                if (instance == null)
                {
                    instance = new RawImageSession();
                }
                return instance;
            }
        }

        public void UpdateRawImageBytes()
        {
            GetRawImage(this.pixelHandle.AddrOfPinnedObject(), this.tex.width, this.tex.height);
            this.tex.SetPixels32(this.pixel32);
            this.tex.Apply();
        }

        public Texture2D GetCurrent2DTexture()
        {
            return this.tex;
        }

    }

    public sealed class RawMaskSession
    {

        private static RawMaskSession instance = null;
        private static readonly object padlock = new object();
        private Texture2D tex;
        private Color32[] pixel32;
        private GCHandle pixelHandle;

        private RawMaskSession()
        {
            this.tex = new Texture2D(
                ImageInputInterface.GetRawImageWidth(),
                ImageInputInterface.GetRawImageHeight(),
                TextureFormat.ARGB32,
                false
            );
            this.pixel32 = tex.GetPixels32();
            this.pixelHandle = GCHandle.Alloc(this.pixel32, GCHandleType.Pinned);
        }

        ~RawMaskSession()
        {
            this.pixelHandle.Free();
        }

        public static RawMaskSession GetInstance()
        {
            lock (padlock)
            {
                if (instance == null)
                {
                    instance = new RawMaskSession();
                }
                return instance;
            }
        }

        public void UpdateRawImageBytes()
        {
            GetMaskDetectionOutput(this.pixelHandle.AddrOfPinnedObject(), this.tex.width, this.tex.height);
            this.tex.SetPixels32(this.pixel32);
            this.tex.Apply();
        }

        public Texture2D GetCurrent2DTexture()
        {
            return this.tex;
        }

    }

    public sealed class ButtonImageSession
    {

        private static SortedDictionary<int, ButtonImageSession> instances = null;
        private static readonly object padlock = new object();
        private bool active;
        private int id;
        private Texture2D tex;
        private Color32[] pixel32;
        private GCHandle pixelHandle;

        private ButtonImageSession(int id)
        {
            this.id = id;
            this.tex = new Texture2D(
                ImageInputInterface.GetRawImageWidth(),
                ImageInputInterface.GetRawImageHeight(),
                TextureFormat.ARGB32,
                false
            );
            this.pixel32 = tex.GetPixels32();
            this.pixelHandle = GCHandle.Alloc(this.pixel32, GCHandleType.Pinned);
            AddBox(this.id);
            this.active = true;
        }

        ~ButtonImageSession()
        {
            this.Deactivate();
        }

        public static ButtonImageSession GetInstance(int id)
        {
            lock (padlock)
            {
                if (instances == null)
                {
                    instances = new SortedDictionary<int, ButtonImageSession>();
                }
                if (!instances.ContainsKey(id))
                {
                    instances[id] = new ButtonImageSession(id);
                }
                else if (!instances[id].active)
                {
                    instances[id] = new ButtonImageSession(id);
                }
                return instances[id];
            }
        }

        public static void DeactivateAll()
        {
            if (instances != null)
            {
                foreach (var instance in instances)
                {
                    instance.Value.Deactivate();
                }
            }
        }

        public void Deactivate()
        {
            this.pixelHandle.Free();
            RemoveBox(this.id);
            this.active = false;
        }

        public void UpdateRawMaskImageBytes()
        {
            GetRawButtonMaskImage(this.id, this.pixelHandle.AddrOfPinnedObject(), this.tex.width, this.tex.height);
            this.tex.SetPixels32(this.pixel32);
            this.tex.Apply();
        }

        public void UpdateRawImageBytes()
        {
            GetRawButtonImage(this.id, this.pixelHandle.AddrOfPinnedObject(), this.tex.width, this.tex.height);
            this.tex.SetPixels32(this.pixel32);
            this.tex.Apply();
        }

        public Texture2D GetCurrent2DTexture()
        {
            return this.tex;
        }

        public double GetValue()
        {
            return GetBoxValue(this.id);
        }
    }
}

