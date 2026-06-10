const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const newmind = b.addModule("newmind", .{
        .root_source_file = b.path("newmind/root.zig"),
        .target = target,
        .optimize = optimize,
    });

    const basic = b.addExecutable(.{
        .name = "basic",
        .root_module = b.createModule(.{
            .root_source_file = b.path("./examples/basic/main.zig"),
            .target = target,
            .optimize = optimize,
            .imports = &.{
                .{ .name = "newmind", .module = newmind },
            },
        }),
    });

    b.installArtifact(basic); // writes to (default) zig-out

    const basic_run = b.addRunArtifact(basic); // creates a run step
    basic_run.step.dependOn(b.getInstallStep());

    const basic_step = b.step("basic", "Run basic example");
    basic_step.dependOn(&basic_run.step);
}
