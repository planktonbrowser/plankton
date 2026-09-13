use ash::vk::Handle;
use std::sync::OnceLock;
use wgpu::hal::api::Vulkan;

pub struct WgpuState {
    pub device: wgpu::Device,
    pub queue: wgpu::Queue,
    pub render_target: wgpu::Texture,
}

static WGPU_STATE: OnceLock<WgpuState> = OnceLock::new();

impl WgpuState {
    pub async fn new() {
        const TEXTURE_DIMS: (usize, usize) = (512, 512);

        let instance = wgpu::Instance::default();
        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions::default())
            .await
            .unwrap();
        let (device, queue) = adapter
            .request_device(&wgpu::DeviceDescriptor {
                label: None,
                required_features: wgpu::Features::empty(),
                required_limits: wgpu::Limits::downlevel_defaults(),
                experimental_features: wgpu::ExperimentalFeatures::disabled(),
                memory_hints: wgpu::MemoryHints::MemoryUsage,
                trace: wgpu::Trace::Off,
            })
            .await
            .unwrap();

        let shader = device.create_shader_module(wgpu::include_wgsl!("shader.wgsl"));

        let render_target = device.create_texture(&wgpu::TextureDescriptor {
            label: None,
            size: wgpu::Extent3d {
                width: TEXTURE_DIMS.0 as u32,
                height: TEXTURE_DIMS.1 as u32,
                depth_or_array_layers: 1,
            },
            mip_level_count: 1,
            sample_count: 1,
            dimension: wgpu::TextureDimension::D2,
            format: wgpu::TextureFormat::Rgba8UnormSrgb,
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT
                | wgpu::TextureUsages::COPY_SRC
                | wgpu::TextureUsages::TEXTURE_BINDING,
            view_formats: &[wgpu::TextureFormat::Rgba8UnormSrgb],
        });

        let pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
            label: None,
            layout: None,
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"),
                compilation_options: Default::default(),
                buffers: &[],
            },
            fragment: Some(wgpu::FragmentState {
                module: &shader,
                entry_point: Some("fs_main"),
                compilation_options: Default::default(),
                targets: &[Some(wgpu::TextureFormat::Rgba8UnormSrgb.into())],
            }),
            primitive: wgpu::PrimitiveState::default(),
            depth_stencil: None,
            multisample: wgpu::MultisampleState::default(),
            multiview_mask: None,
            cache: None,
        });

        log::info!("Wgpu context set up.");

        let texture_view = render_target.create_view(&wgpu::TextureViewDescriptor::default());

        let mut command_encoder =
            device.create_command_encoder(&wgpu::CommandEncoderDescriptor::default());
        {
            let mut render_pass = command_encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: None,
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: &texture_view,
                    depth_slice: None,
                    resolve_target: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(wgpu::Color::GREEN),
                        store: wgpu::StoreOp::Store,
                    },
                })],
                depth_stencil_attachment: None,
                occlusion_query_set: None,
                timestamp_writes: None,
                multiview_mask: None,
            });
            render_pass.set_pipeline(&pipeline);
            render_pass.draw(0..3, 0..1);
        }
        queue.submit(Some(command_encoder.finish()));
        log::info!("Commands submitted.");

        WGPU_STATE.get_or_init(|| Self {
            device,
            queue,
            render_target,
        });
    }
}

pub fn hal_texture() -> u64 {
    if WGPU_STATE.get().is_none() {
        pollster::block_on(WgpuState::new());
    }
    let hal = unsafe {
        WGPU_STATE
            .get()
            .expect("WGPU state empty")
            .render_target
            .as_hal::<Vulkan>()
            .expect("Texture is not running on Vulkan backend")
    };
    let raw_vk_image = unsafe { hal.raw_handle() };
    raw_vk_image.as_raw()
}

#[cxx::bridge]
mod ffi {
    extern "Rust" {
        fn hal_texture() -> u64;
    }
}
