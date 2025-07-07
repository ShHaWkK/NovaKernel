#![allow(clippy::missing_safety_doc)]
#[no_mangle]
pub extern "C" fn sandbox_process(pid: u64) {
    let _ = pid;
}